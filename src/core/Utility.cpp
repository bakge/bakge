/* *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Paul Holden et al. (See AUTHORS)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * */

#include <bakge/Bakge.h>
#include <bakge/internal/Utility.h>
#include <bakge/internal/Graphics.h>
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

static PHYSFS_File* LogFile = NULL;
// Ensure threads have mutual-exclusive log file writes
static Mutex* LogLock = NULL;
// Used to track whether we're in block mode or not
static volatile int LogBlockMode = 0;

Result Init(int argc, char* argv[])
{
    if(PHYSFS_init(argv[0]) == 0) {
        fprintf(stderr, "Error initializing PhysicsFS\n");
        return Deinit();
    }

    if(PHYSFS_addToSearchPath(".", 0) == 0) {
        fprintf(stderr, "Error mounting running directory: %s\n",
                                            PHYSFS_getLastError());
        return Deinit();
    }

    if(PHYSFS_setWriteDir(".") == 0) {
        fprintf(stderr, "Error setting write directory: %s\n",
                                            PHYSFS_getLastError());
        return Deinit();
    }

    LogFile = PHYSFS_openWrite("bakge.log");
    if(LogFile == NULL) {
        fprintf(stderr, "Error opening log file: %s\n", PHYSFS_getLastError());
        return Deinit();
    }

    LogLock = bakge::Mutex::Create();
    if(LogLock == NULL) {
        fprintf(stderr, "Error creating log mutex\n");
        return Deinit();
    }

    /* Run platform-specific initialization protocol */
    if(PlatformInit(argc, argv) != BGE_SUCCESS)
        return Deinit();

    if(!glfwInit()) {
        Log("GLFW initialization failed\n");
        return Deinit();
    }

#ifdef _DEBUG
    glfwSetErrorCallback(GLFWErrorHandler);
#endif // _DEBUG

    /* We don't want the shared context window visible */
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

    /* Create our shared context window */
    Window::SharedContext = glfwCreateWindow(16, 16, "", NULL, NULL);
    if(Window::SharedContext == NULL) {
        Log("Error creating shared context\n");
        return Deinit();
    }

    /* Need to make context current so we can init the shader library */
    glfwMakeContextCurrent(Window::SharedContext);

    /* So future GLFW windows are visible */
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        Log("Error initializing GLEW\n");
        return Deinit();
    }

    // Check for required OpenGL extensions to run Bakge
    if(!CheckRequiredExtensions()) {
        return Deinit();
    }

    if(_GraphicsInit(argc, argv) == BGE_FAILURE) {
        return Deinit();
    }

    /* Initialize our Bakge shader library */
    if(Shader::InitShaderLibrary() != BGE_SUCCESS) {
        Log("Failed to initialize shader library\n");
        return Deinit();
    }

#ifdef _DEBUG
#define BGE_BUILD_CONFIGURATION "[Debug Configuration]"
#else
#define BGE_BUILD_CONFIGURATION "[Release Configuration]"
#endif // _DEBUG

    BeginLogBlock();

    Log("Bakersfield Game Engine v%d.%d.%d %s\n", BGE_VER_MAJ, BGE_VER_MIN,
                                    BGE_VER_REV, BGE_BUILD_CONFIGURATION);
    SystemInfo();

    Log("\n");
    EndLogBlock();

    return BGE_SUCCESS;
}


Result Deinit()
{
    if(LogFile != NULL && PHYSFS_close(LogFile) == 0) {
        fprintf(stderr, "Error closing log file\n");
    }

    if(PHYSFS_isInit() != 0)
        PHYSFS_deinit();

    if(LogLock != NULL)
        delete LogLock;

    /* Run platform-specific deinitialization protocol */
    PlatformDeinit();

    Shader::DeinitShaderLibrary();

    /* Destroy our shared context window */
    if(Window::SharedContext != NULL)
        glfwDestroyWindow(Window::SharedContext);

    glfwTerminate();

    return BGE_FAILURE;
}


void SystemInfo()
{
    extern void PlatformSystemInfo();

    PlatformSystemInfo();

    Log("OpenGL v%s\n", glGetString(GL_VERSION));
    Log("  - %s\n", glGetString(GL_VENDOR));
    Log("  - %s\n", glGetString(GL_RENDERER));
    Log("  - GLSL v%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
}


Result BeginLogBlock()
{
    Result Res = LogLock->Lock();
    if(LogBlockMode == 0) {
        LogBlockMode = 1;
    } else {
        Log("WARNING: BeginLogBlock called while already in block mode.\n");
    }

    return Res;
}


Result EndLogBlock()
{
    if(LogBlockMode == 1) {
        LogBlockMode = 0;
    } else {
        Log("WARNING: EndLogBlock called while not in block mode.\n");
    }

    return LogLock->Unlock();
}


int Log(const char* Format, ...)
{
    va_list ArgList;

#define BGE_LOG_BUFSZ 4096
    static char Buf[BGE_LOG_BUFSZ];

    va_start(ArgList, Format);

    bakge::Microseconds Time = bakge::GetRunningTime();

    // 3.6 billion microseconds per hour
    uint64 Hour = Time / 3600000000U;

    // Remaining microseconds (this amounts to less than one hour)
    uint32 Rem = Time % 3600000000U;
    uint32 Min = Rem / 60000000;

    // Remaining microseconds now amounts to less than one minute
    Rem %= 60000000;
    uint32 Sec = Rem / 1000000;

    // Our remaining microseconds converted to milliseconds
    Rem %= 1000000;
    Rem /= 1000;

    LogLock->Lock();

#ifdef _WIN32
#define BGE_LOG_TIMESTAMP_FMT "[%02llu:%02u:%02u.%03u] "
#else
#define BGE_LOG_TIMESTAMP_FMT "[%02lu:%02u:%02u.%03u] "
#endif // _WIN32

    va_list ArgList2;

#ifdef _WIN32
#define va_copy(A, B) ((A) = (B))
#endif // _WIN32

    va_copy(ArgList2, ArgList);

    // First print to stdout to grab required length of buffer
    int Len = fprintf(stdout, BGE_LOG_TIMESTAMP_FMT, Hour, Min, Sec, Rem);
    Len += vfprintf(stdout, Format, ArgList);

    // If we have enough space, write to buffer and write to log
    if(Len < BGE_LOG_BUFSZ) {
        int TimestampLen = sprintf(Buf, BGE_LOG_TIMESTAMP_FMT, Hour,
                                                     Min, Sec, Rem);
        vsprintf(Buf + TimestampLen, Format, ArgList2);

        PHYSFS_sint64 Error = PHYSFS_write(LogFile, Buf, 1, Len);
        if(Error < 0) {
            fprintf(stderr, "Error writing to log: %s\n",
                                    PHYSFS_getLastError());
            return -1;
        } else if(Error < Len) {
            fprintf(stderr, "Incomplete write to log: %s\n",
                                    PHYSFS_getLastError());
        }
    }

    // Always flush in case a crash happens
    if(PHYSFS_flush(LogFile) == 0)
        fprintf(stderr, "Error flushing log: %s\n", PHYSFS_getLastError());

    LogLock->Unlock();

    va_end(ArgList);
    va_end(ArgList2);

    return Len;
}

} /* bakge */
