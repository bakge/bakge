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

#include <stdio.h>
#include <stdlib.h>
#include <bakge/Bakge.h>

int main(int argc, char* argv[])
{
    bakge::Window* Win;
    bakge::Cube* Obj;
    bakge::Texture* Tex;
    bakge::Crowd* Group;
    bakge::Camera3D* Cam;
    bakge::AudioContext* AC;
    bakge::Source* Src;
    bakge::Stream* Str;

    printf("Initializing Bakge\n");
    bakge::Init(argc, argv);

    GLubyte* Bitmap = new GLubyte[512 * 512 * 3];
    memset((void*)Bitmap, 0, sizeof(Bitmap[0]) * 512 * 512 * 3);

    Win = bakge::Window::Create(1024, 768, 0);
    if(Win == NULL) {
        printf("Error creating window\n");
        return bakge::Deinit();
    }

    glClearColor(1, 0, 1, 1);
    glEnable(GL_DEPTH_TEST);

    /* Create simple checkerboard texture */
    for(int i=0;i<256;++i) {
        for(int j=0;j<256;++j) {
            Bitmap[3*(i*512+j)] = 255;
            Bitmap[3*(i*512+j)+1] = 255;
            Bitmap[3*(i*512+j)+2] = 255;
        }
    }

    for(int i=256;i<512;++i) {
        for(int j=256;j<512;++j) {
            Bitmap[3*(i*512+j)] = 255;
            Bitmap[3*(i*512+j)+1] = 255;
            Bitmap[3*(i*512+j)+2] = 255;
        }
    }

    Tex = bakge::Texture::Create(512, 512, NULL, GL_RGB, GL_UNSIGNED_BYTE,
                                                            (void*)Bitmap);

    Obj = bakge::Cube::Create();

#define CROWD_SIZE 50
    Group = bakge::Crowd::Create(CROWD_SIZE);

    srand(time(0));

    AC = bakge::AudioContext::Create();
    AC->Bind();

    PHYSFS_addToSearchPath("C:/", 0);
    PHYSFS_file* F = PHYSFS_openRead("door_gate_bar_locked.ogg");
    int Len = (int)PHYSFS_fileLength(F);
    bakge::Byte* Data = (bakge::Byte*)malloc(Len);
    PHYSFS_read(F, (void*)Data, 1, Len);

    Src = bakge::Source::Create();
    if(Src == NULL) {
        printf("Couldn't create Source\n");
        return -1;
    }

    Str = bakge::Stream::Create(Len, Data);
    if(Str == NULL) {
        printf("Couldn't create Stream\n");
        return -2;
    }

    if(Src->Attach(Str) == BGE_FAILURE) {
        printf("Couldn't attach stream to source\n");
        return -3;
    }

#define TRAND ((((float)(rand() % 1000) / 1000) * 5) - 2.5f)
#define RRAND (((float)(rand() % 1000)) / 1000) * 6.28f
#define SRAND (((float)(rand() % 1000) / 1000.0f) + 0.5f)
    for(int i=0;i<CROWD_SIZE;++i) {
        Group->ScaleMember(i, SRAND, SRAND, SRAND);
        Group->RotateMember(i, bakge::Quaternion::FromEulerAngles(RRAND,
                                                        RRAND, RRAND));
        Group->TranslateMember(i, TRAND, TRAND, TRAND);
    }

    Obj->SetDrawStyle(bakge::MESH_DRAW_STYLE_SOLID);

    Cam = new bakge::Camera3D;
    Cam->SetPosition(0, 0.5f, 1.25f);
    Cam->SetTarget(0, 0, 0);

    float Rot = 0;
    bakge::Microseconds NowTime;
    bakge::Microseconds LastTime = bakge::GetRunningTime();

    Src->Play();

    while(1) {
        /* Poll events for all windows */
        bakge::Window::PollEvents();

        /* Don't draw if the window has closed */
        if(Win->IsOpen() == false)
            break;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        NowTime = bakge::GetRunningTime();
        float DeltaTime = (float)(NowTime - LastTime) / 1000000;
        LastTime = NowTime;

        for(int i=0;i<CROWD_SIZE;++i)
            Group->RotateMember(i, bakge::Quaternion::FromEulerAngles(0,
                                                        DeltaTime, 0));

        Cam->Bind();
        Tex->Bind();
        Obj->Bind();
        Group->Bind();
        Obj->DrawInstanced(Group->GetPopulation()); /* No renderer for now */
        Group->Unbind();
        Obj->Unbind();
        Tex->Unbind();
        Cam->Unbind();

        Win->SwapBuffers();
    }

    if(Win != NULL)
        delete Win;

    if(Obj != NULL)
        delete Obj;

    if(Tex != NULL)
        delete Tex;

    if(Group != NULL)
        delete Group;

    if(Cam != NULL)
        delete Cam;

    AC->Unbind();
    delete AC;

    printf("Deinitializing Bakge\n");
    bakge::Deinit();

    delete[] Bitmap;

    return 0;
}
