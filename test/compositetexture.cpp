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
#include "TestEngine.h"

bakge::Rectangle* Obj;
bakge::Pawn* It;
bakge::CompositeTexture* Tex;
GLint ShaderProgram;
bakge::Camera2D* UICam;
GLubyte* Bitmap;

float Rot;
bakge::Microseconds NowTime;
bakge::Microseconds LastTime;

bakge::Result InitTest()
{
    UICam = new bakge::Camera2D;

    Rot = 0;
    LastTime = bakge::GetRunningTime();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int W, H, N;
    bakge::Byte* BL;
    bakge::Byte* TL;
    bakge::Byte* BR;
    bakge::Byte* TR;
    bakge::Byte* VG;
    bakge::Byte* HG;
    bakge::Byte* F;

    PHYSFS_addToSearchPath("C:/", 0);

    bakge::DecodeImageFile("button_f.png", &F, &W, &H, &N);
    bakge::DecodeImageFile("button_bl.png", &BL, &W, &H, &N);
    bakge::DecodeImageFile("button_tl.png", &TL, &W, &H, &N);
    bakge::DecodeImageFile("button_br.png", &BR, &W, &H, &N);
    bakge::DecodeImageFile("button_tr.png", &TR, &W, &H, &N);
    bakge::DecodeImageFile("button_vg.png", &VG, &W, &H, &N);
    bakge::DecodeImageFile("button_hg.png", &HG, &W, &H, &N);

    Tex = bakge::CompositeTexture::Create(100, 32, W, BL, TL, TR,
                                                BR, HG, VG, F);

    It = bakge::Pawn::Create();
    Obj = bakge::Rectangle::Create(100, 32);

    Obj->SetDrawStyle(bakge::SHAPE_DRAW_STYLE_SOLID);

    It->SetPosition(150, 150, 0);

    UICam->SetPosition(0, 0, -1.0f);
    UICam->SetSpan(600.0f, 400.0f, 2.0f);

    return BGE_SUCCESS;
}


bakge::Result PreRenderTest()
{
    UICam->Bind();
    Tex->Bind();
    Obj->Bind();
    It->Bind();

    return BGE_SUCCESS;
}


bakge::Result RenderTest()
{
    Obj->Draw();

    return BGE_SUCCESS;
}


bakge::Result PostRenderTest()
{
    It->Unbind();
    Obj->Unbind();
    Tex->Unbind();
    UICam->Unbind();

    return BGE_SUCCESS;
}


bakge::Result ShutDownTest()
{
    if(Obj != NULL)
        delete Obj;

    if(Tex != NULL)
        delete Tex;

    if(It != NULL)
        delete It;

    if(UICam != NULL)
        delete UICam;

    delete[] Bitmap;

    return BGE_SUCCESS;
}


int main(int argc, char* argv[])
{
    bakge::Init(argc, argv);

    bakge::TestEngine* RectTest = new bakge::TestEngine;

    RectTest->SetInitializeCallback(InitTest);
    RectTest->SetPreRenderCallback(PreRenderTest);
    RectTest->SetRenderCallback(RenderTest);
    RectTest->SetPostRenderCallback(PostRenderTest);
    RectTest->SetShutDownCallback(ShutDownTest);

    RectTest->StartEngine();

    delete RectTest;

    bakge::Deinit();

    return 0;
}
