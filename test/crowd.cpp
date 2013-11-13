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

static const char* Vertex =
    "varying float LightIntensity;\n"
    "varying vec2 TexCoord0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 VertexPosition = bge_View * bge_Model * bge_InstanceModel * bge_Vertex;\n"
    "\n"
    "    mat3x3 NormalMatrix = mat3x3(\n"
    "        normalize(vec3(bge_InstanceModel[0].xyz)),\n"
    "        normalize(vec3(bge_InstanceModel[1].xyz)),\n"
    "        normalize(vec3(bge_InstanceModel[2].xyz))\n"
    "    );\n"
    "\n"
    "    TexCoord0 = bge_TexCoord;\n"
    "\n"
    "    vec3 VertexNormal = NormalMatrix * vec3(bge_Normal.xyz);\n"
    "    LightIntensity = dot(normalize(VertexNormal), vec3(0, 0, 1));\n"
    "\n"
    "    gl_Position = bge_Projection * VertexPosition;\n"
    "}\n"
    "\n";

static const char* Fragment =
    "varying float LightIntensity;\n"
    "varying vec2 TexCoord0;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    vec4 Diffuse = texture2D(bge_Diffuse, TexCoord0);\n"
    "    vec4 FinalColor = vec4(0.2f, 0.2f, 0.2f, 1.0f) * Diffuse;\n"
    "    FinalColor += Diffuse * max(LightIntensity, 0);\n"
    "\n"
    "    gl_FragColor = FinalColor;\n"
    "}\n"
    "\n";

int main(int argc, char* argv[])
{
    bakge::Window* Win;
    bakge::Cube* Obj;
    bakge::Texture* Tex;
    bakge::Crowd* Group;
    bakge::Camera3D* Cam;
    bakge::Shader* Sh;

    bakge::Init(argc, argv);

    GLubyte* Bitmap = new GLubyte[512 * 512 * 3];
    memset((void*)Bitmap, 0, sizeof(Bitmap[0]) * 512 * 512 * 3);

    Win = bakge::Window::Create(1024, 768, 0);
    if(Win == NULL) {
        bakge::Log("test/crowd: Error creating window\n");
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

#define CROWD_SIZE 5000
    Group = bakge::Crowd::Create(CROWD_SIZE);

    srand(time(0));

#define TRAND ((((float)(rand() % 1000) / 1000) * 5) - 2.5f)
#define RRAND (((float)(rand() % 1000)) / 1000) * 6.28f
#define SRAND (((float)(rand() % 1000) / 1000.0f) + 0.5f)
    for(int i=0;i<CROWD_SIZE;++i) {
        Group->ScaleMember(i, SRAND / 10, SRAND / 10, SRAND / 10);
        Group->RotateMember(i, bakge::Quaternion::FromEulerAngles(RRAND,
                                                        RRAND, RRAND));
        Group->TranslateMember(i, TRAND, TRAND, TRAND);
    }

    Obj->SetDrawStyle(bakge::SHAPE_DRAW_STYLE_SOLID);

    Cam = new bakge::Camera3D;
    Cam->SetPosition(0, 0.5f, 1.25f);
    Cam->SetTarget(0, 0, 0);

    Sh = bakge::Shader::LoadFromStrings(1, 1, &Vertex, &Fragment);

    Sh->Bind();

    float Rot = 0;
    bakge::Microseconds NowTime;
    bakge::Microseconds LastTime = bakge::GetRunningTime();

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

    if(Sh != NULL)
        delete Sh;

    bakge::Deinit();

    delete[] Bitmap;

    return 0;
}
