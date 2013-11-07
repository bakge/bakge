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

#include <bakge/gdk/engine/Common.h>
#include <pugi/pugixml.hpp>

namespace bakge
{
namespace gdk
{

#define BGE_GDK_ENGINE_CONFIG_DIR "Bakge"
#define BGE_GDK_ENGINE_CONFIG_FILE "main.bcf"
#define BGE_CONFIG_DEFAULT_WINDOW_WIDTH 800
#define BGE_CONFIG_DEFAULT_WINDOW_HEIGHT 600

Application::Application()
{
    Gui = NULL;
    Win = NULL;
}


Application::~Application()
{
    if(Gui != NULL)
        delete Gui;

    if(Win != NULL)
        delete Win;
}


Application* Application::Create()
{
    Application* App = new Application;
    if(App == NULL) {
        Log("ERROR: Application - Couldn't allocate memory.\n");
        return NULL;
    }

    return App;
}


Result Application::Initialize()
{
    glClearColor(0.85f, 0.85f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const char* UserDir = PHYSFS_getUserDir();
    if(UserDir == NULL) {
        Log("  ERROR: Couldn't find user home directory.\n");
        return BGE_FAILURE;
    }

    Log("  - Found user home directory \"%s\"\n", UserDir);

    if(PHYSFS_addToSearchPath(UserDir, 0) == 0) {
        Log("  ERROR: Couldn't add user home directory to search path.\n");
        Log("    %s\n", PHYSFS_getLastError());
        return BGE_FAILURE;
    }

    int Len = strlen(UserDir) + strlen(BGE_GDK_ENGINE_CONFIG_DIR);
    char* DirPath = (char*)malloc(Len + 1);
    sprintf(DirPath, "%s%s", UserDir, BGE_GDK_ENGINE_CONFIG_DIR);

    if(PHYSFS_addToSearchPath(DirPath, 0) == 0) {
        Log("  ERROR: Couldn't add Bakge directory to search path.\n");
        Log("    %s\n", PHYSFS_getLastError());
        return BGE_FAILURE;
    }

    Len += strlen(BGE_GDK_ENGINE_CONFIG_FILE);
    char* ConfigPath = (char*)malloc(Len + 2);
    sprintf(ConfigPath, "%s\\%s", DirPath, BGE_GDK_ENGINE_CONFIG_FILE);

    Log("  - Searching for file \"%s\"\n", ConfigPath);

    uint32 Width = BGE_CONFIG_DEFAULT_WINDOW_WIDTH;
    uint32 Height = BGE_CONFIG_DEFAULT_WINDOW_HEIGHT;
    bool FoundConfig = false;

    if(PHYSFS_exists(BGE_GDK_ENGINE_CONFIG_FILE)) {
        if(PHYSFS_isDirectory(BGE_GDK_ENGINE_CONFIG_FILE) == 0) {
            Log("  - Found file \"%s\"\n", ConfigPath);
            FoundConfig = true;
        } else {
            Log("  - \"%s\" is a directory.\n", ConfigPath);
        }
    } else {
        Log("  - File \"%s\" not found.\n", ConfigPath);
    }

    pugi::xml_document Config;
    if(FoundConfig) {
        Log("  - Loading config file...\n");
        //TODO: Use PhysFS and buffer file contents?
        pugi::xml_parse_result Load = Config.load_file(ConfigPath);
        Log("  - pugixml document load result: %s\n", Load.description());
        Log("  - Scanning for \"window\" node...\n");
        pugi::xml_node WindowNode = Config.child("window");
        if(WindowNode.empty()) {
            Log("  ERROR: \"window\" node not found.\n");
            return BGE_FAILURE;
        } else {
            Log("  - Found \"window\" node, extracting dimensions info.\n");
        }
        pugi::xml_attribute Attr = WindowNode.attribute("width");
        Height = WindowNode.attribute("height").as_int(-1);
        if(Attr.empty()) {
            Log("  ERROR: \"width\" attribute not found.\n");
            return BGE_FAILURE;
        } else {
            // Get value as int; return -1 if value is not an int
            Width = Attr.as_int(-1);
            if(Width < 0) {
                Log("  ERROR: \"width\" attribute not a number.\n");
                return BGE_FAILURE;
            } else {
                Log("  - Found \"width\" attribute: %d\n", Width);
            }
        }
        Attr = WindowNode.attribute("height");
        if(Attr.empty()) {
            Log("  ERROR: \"height\" attribute not found.\n");
            return BGE_FAILURE;
        } else {
            Height = Attr.as_int(-1);
            if(Height < 0) {
                Log("  ERROR: \"height\" attribute not a number.\n");
                return BGE_FAILURE;
            } else {
                Log("  - Found \"height\" attribute: %d\n", Height);
            }
        }
    } else {
        Log("  - No config file found. Creating a default.\n");
        pugi::xml_node Node = Config.append_child("window");
        if(Node.empty()) {
            Log("  ERROR: Couldn't create \"window\" node.\n");
            return BGE_FAILURE;
        }
        // Create a width attribute on our window node.
        pugi::xml_attribute Attr = Node.append_attribute("width");
        if(Attr.empty()) {
            Log("  ERROR: Couldn't create \"width\" attribute.\n");
            return BGE_FAILURE;
        } else {
            Attr.set_value(BGE_CONFIG_DEFAULT_WINDOW_WIDTH);
        }
        // Now make a height attribute
        Attr = Node.append_attribute("height");
        if(Attr.empty()) {
            Log("  ERROR: Couldn't create \"height\" attribute.\n");
            return BGE_FAILURE;
        } else {
            Attr.set_value(BGE_CONFIG_DEFAULT_WINDOW_HEIGHT);
        }
        // Save document to file
        if(Config.save_file(ConfigPath, "  ") == false) {
            Log("  ERROR: Couldn't write to file \"%s\"\n", ConfigPath);
        }
    }

    free(DirPath);
    free(ConfigPath);

    Win = Window::Create(Width, Height, 0);
    if(Win == NULL) {
        Log("  ERROR: Couldn't create Window\n");
        return BGE_FAILURE;
    } else {
        Log("  - Created Window\n");
        Log("    - %d x %d\n", Width, Height);
        //TODO: Update when full-screen window support added
        Log("    - %s\n", "Windowed");
    }

    Win->SetEventHandler(this);

    Gui = GUI::Create(Width, Height);
    if(Gui == NULL) {
        Log("  ERROR: Couldn't create GUI\n");
        return BGE_FAILURE;
    } else {
        Log("  - Created GUI\n");
        Log("    - %d x %d\n", Width, Height);
    }

    return BGE_SUCCESS;
}


Result Application::ShutDown()
{
    if(Win->IsOpen())
        Win->Close();

    return BGE_SUCCESS;
}


int Application::Run()
{
    Microseconds T1, T2;
    T1 = GetRunningTime();

    while(1) {
        Window::PollEvents();

        if(Win->IsOpen() == false)
            break;

        PreRenderStage();
        RenderStage();
        PostRenderStage();

        T2 = GetRunningTime();

        Update(((Scalar)(T2 - T1)) / 1000000);

        T1 = T2;
    }

    return 0;
}


Result Application::Update(Seconds DeltaTime)
{
    return BGE_SUCCESS;
}


Result Application::PreRenderStage()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    return BGE_SUCCESS;
}


Result Application::PostRenderStage()
{
    Win->SwapBuffers();

    return BGE_SUCCESS;
}


Result Application::RenderStage()
{
    return BGE_SUCCESS;
}


Result Application::KeyEvent(KeyID K, KeyState S, ScanCode C, ModField M)
{
    return BGE_SUCCESS;
}


Result Application::MouseEvent(ButtonID B, ButtonState S, ModField M)
{
    return BGE_SUCCESS;
}


Result Application::MotionEvent(Motion X, Motion Y)
{
    return BGE_SUCCESS;
}


Result Application::ScrollEvent(Motion X, Motion Y)
{
    return BGE_SUCCESS;
}


Result Application::CloseEvent()
{
    return BGE_SUCCESS;
}

} /* gdk */
} /* bakge */
