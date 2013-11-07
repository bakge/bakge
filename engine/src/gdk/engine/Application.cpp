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


// Scans user's home directory for the main configuration file. Every
// Application loads up this config. Whether or not specialized applications
// extract and use info from this file depends on the implementation of
// Application::Initialize(). This method is only used internally in this
// base class Initialize implementation.
//
// This method expects it is called in a Log block, so it doesn't create one
// of its own.
//
// Returns pointer to string (free this when done) representing path of config
// file.
static char* _FindMainConfig()
{
    // Ask PhysFS for the user's home directory
    const char* UserDir = PHYSFS_getUserDir();
    if(UserDir == NULL) {
        Log("  ERROR: Couldn't find user home directory.\n");
        return NULL;
    }

    // Found home directory; add it to search path for PhysFS
    Log("  - Found user home directory\n", UserDir);
    if(PHYSFS_addToSearchPath(UserDir, 0) == 0) {
        Log("  ERROR: Couldn't add user home directory to search path.\n");
        Log("    %s\n", PHYSFS_getLastError());
        return NULL;
    }

    // DirPath will be a string representing the directory our desired config
    // resides in
    int Len = strlen(UserDir) + strlen("Bakge");
    char* DirPath = (char*)malloc(Len + 1);
    sprintf(DirPath, "%sBakge", UserDir);

    // ConfigPath is the full string path of the config file, so we can
    // pass that to pugi for loading.
    Len += strlen("main.config");
    char* ConfigPath = (char*)malloc(Len + 2);
    sprintf(ConfigPath, "%s\\main.config", DirPath);

    // Need to add the Bakge subdirectory to the search path as well.
    // PhysFS gets antsy when it comes to partial/full path names.
    if(PHYSFS_addToSearchPath(DirPath, 0) == 0) {
        Log("  ERROR: Couldn't add Bakge directory to search path.\n");
        Log("    %s\n", PHYSFS_getLastError());
        goto _MainConfigNotFound; // Bite me
    }

    // Now that home directory is in search path, search for it
    Log("  - Searching for file \"main.config\"\n");
    if(PHYSFS_exists("main.config") == 0) {
        Log("  - File \"main.config\" not found.\n");
        goto _MainConfigNotFound; // Stop looking at me like that
    }

    if(PHYSFS_isDirectory("main.config") == 0) {
        Log("  - Found \"main.config\" file.\n");
    } else {
        Log("  - \"main.config\" is a directory.\n");
    }

_MainConfigNotFound:

    free(DirPath);

    return ConfigPath;
}


// Load and extract info from the main config. Hidden method, only used
// internally in this base class Initialize implementation. If it fails to
// find certain data, it will set the corresponding config values to 0.
//
// This method expects it is called in a Log block, so it doesn't create one
// of its own.
static Result _LoadMainConfig(const char* Path, MainConfig* Config)
{
    Result LoadResult = BGE_SUCCESS;

    pugi::xml_document Doc;

    // Load the document from path
    pugi::xml_parse_result Load = Doc.load_file(Path);
    if(Load.status != pugi::status_ok) {
        Log("  ERROR: Couldn't load config file.\n");
        return BGE_FAILURE;
    }

    Log("  - Loading config file...%s\n", Load.description());

    // Find window node.
    Log("  - Searching for \"window\" node...\n");
    pugi::xml_node WindowNode = Doc.child("window");
    if(WindowNode.empty()) {
        Log("  WARNING: \"window\" node not found.\n");
        LoadResult = BGE_FAILURE;
    } else {
        Log("  - Found \"window\" node, extracting dimensions info.\n");
    }

    // pugixml is swell in that empty nodes return empty attributes/child
    // nodes if they themselves are empty.
    pugi::xml_attribute Attr = WindowNode.attribute("width");
    if(Attr.empty()) {
        Log("  WARNING: \"width\" attribute not found.\n");
        Config->Win.Width = 0;
        LoadResult = BGE_FAILURE;
    } else {
        // Get value as int; return -1 if value is not an int
        Config->Win.Width = Attr.as_int(0);
        if(Config->Win.Width == 0) {
            Log("  WARNING: \"width\" attribute not a number.\n");
        }
    }

    Attr = WindowNode.attribute("height");
    if(Attr.empty()) {
        Log("  WARNING: \"height\" attribute not found.\n");
        Config->Win.Height = 0;
        LoadResult = BGE_FAILURE;
    } else {
        Config->Win.Height = Attr.as_int(0);
        if(Config->Win.Height == 0) {
            Log("  WARNING: \"height\" attribute not a number.\n");
        }
    }

    return LoadResult;
}


// Creates a default config file with the given path, also setting the values
// of a MainConfig struct to those same defaults written to file.
Result _CreateDefaultMainConfig(const char* Path, MainConfig* Config)
{
    Log("  - No config file found. Creating a default.\n");

    pugi::xml_document Doc;

    Result CreateResult = BGE_SUCCESS;

    pugi::xml_node Node = Doc.append_child("window");
    if(Node.empty()) {
        Log("  ERROR: Couldn't create \"window\" node.\n");
        CreateResult = BGE_FAILURE;
    }

    // Create a width attribute on our window node.
    pugi::xml_attribute Attr = Node.append_attribute("width");
    if(Attr.empty()) {
        Log("  ERROR: Couldn't create \"width\" attribute.\n");
        CreateResult = BGE_FAILURE;
    }
    // Set its value
    if(Attr.set_value(BGE_CONFIG_DEFAULT_WINDOW_WIDTH) == false) {
        Log("  ERROR: Couldn't set \"width\" attribute.\n");
        CreateResult = BGE_FAILURE;
    } else {
        Config->Win.Width = BGE_CONFIG_DEFAULT_WINDOW_WIDTH;
    }

    // Now make a height attribute
    Attr = Node.append_attribute("height");
    if(Attr.empty()) {
        Log("  ERROR: Couldn't create \"height\" attribute.\n");
        CreateResult = BGE_FAILURE;
    }
    // Set its value
    if(Attr.set_value(BGE_CONFIG_DEFAULT_WINDOW_HEIGHT) == false) {
        Log("  ERROR: Couldn't set \"height\" attribute.\n");
        CreateResult = BGE_FAILURE;
    } else {
        Config->Win.Height = BGE_CONFIG_DEFAULT_WINDOW_HEIGHT;
    }

    // Save document to file
    Log("  - Saving default config to \"%s\"\n", Path);
    if(Doc.save_file(Path, "  ") == false) {
        Log("  ERROR: Couldn't write to file \"%s\"\n", Path);
        CreateResult = BGE_FAILURE;
    }

    Log("  - Done.\n");

    return CreateResult;
}

Result Application::Initialize()
{
    ClearFlags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;

    glClearColor(0.85f, 0.85f, 0.7f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    BeginLogBlock();
	
    MainConfig Config;
    char* Path = _FindMainConfig();
    // If file couldn't be found, make a default one.
    if(Path == NULL) {
        _CreateDefaultMainConfig(Path, &Config);
    } else {
        // Load found config
        _LoadMainConfig(Path, &Config);
    }

    // Analyze the config results. If any are invalid, use some sensible
    // defaults instead.
    if(Config.Win.Width == 0) {
        Log("  - Using default window width.\n");
        Config.Win.Width = BGE_CONFIG_DEFAULT_WINDOW_WIDTH;
    }

    if(Config.Win.Height == 0) {
        Log("  - Using default window height.\n");
        Config.Win.Height = BGE_CONFIG_DEFAULT_WINDOW_HEIGHT;
    }

    free(Path);

    Win = Window::Create(Config.Win.Width, Config.Win.Height, 0);
    if(Win == NULL) {
        EndLogBlock();
        return BGE_FAILURE;
    } else {
        Log("  - Created Window\n");
        Log("    - %d x %d\n", Config.Win.Width, Config.Win.Height);
        //TODO: Update when full-screen window support added
        Log("    - %s\n", "Windowed");
    }

    Win->SetEventHandler(this);

    Gui = GUI::Create(Config.Win.Width, Config.Win.Height);
    if(Gui == NULL) {
        EndLogBlock();
        return BGE_FAILURE;
    } else {
        Log("  - Created GUI\n");
        Log("    - %d x %d\n", Config.Win.Width, Config.Win.Height);
    }

    EndLogBlock();

    return BGE_SUCCESS;
}

// Undefine these; no longer needed
#undef BGE_CONFIG_DEFAULT_WINDOW_WIDTH
#undef BGE_CONFIG_DEFAULT_WINDOW_HEIGHT

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
    glClear(ClearFlags);

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
