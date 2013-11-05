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

bakge::GlyphMap* Tex;
bakge::Camera2D* UICam;
bakge::Font* F;
bakge::Stamp* St;
bakge::Shader* Shad;

static const char* Vertex =
    "varying vec2 TexCoord0;\n"
    "varying vec4 Position;\n"
    "void main()\n"
    "{\n"
    "    TexCoord0 = bge_TexCoord;\n"
    "    Position = bge_View * bge_Vertex;\n"
    "    gl_Position = bge_Projection * Position;\n"
    "}\n"
    "\n";

static const char* Fragment =
    "varying vec2 TexCoord0;\n"
    "varying vec4 Position;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(bge_Diffuse, TexCoord0);\n"
    "}\n"
    "\n";

bakge::Result InitTest()
{
    UICam = new bakge::Camera2D;

    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    Shad = bakge::Shader::LoadFromStrings(1, 1, &Vertex, &Fragment);
    Shad->Bind();

    PHYSFS_addToSearchPath("C:/", 0);
    F = bakge::Font::Load("arial.ttf");

    int R = F->Bake(&Tex, 4, 132, 20.0f);
    bakge::Log("test/font: Bake result %d\n", R);

    UICam->SetPosition(-256.0f, 256.0f, -1.0f);
    UICam->SetSpan(600.0f, -400.0f, 2.0f);

    St = bakge::Stamp::Create();
    St->SetDimensions(50, 50);

    return BGE_SUCCESS;
}


bakge::Result PreRenderTest()
{
    UICam->Bind();
    Tex->Bind();

    St->Begin(-200, 0);
    St->Bind();

    return BGE_SUCCESS;
}


bakge::Result RenderTest()
{
    const char* Str = "The Quick Brown Fox Jumps Over the Lazy Dog,\n"
                      "But Alas, He Has Tripped and Hurt His Paw.\n\n"
                      "Despite The Dog-Wife's Best Efforts, He Lazes On.\n"
                      "He Is Distraught For His Dog-Son Has Left Him;\n"
                      "And Embarked On His Own Lazy Journey.\n";
    const char* C = Str;
    while(*C) {
        if(*C == '\n') {
            St->EndLine();
        } else {
            Tex->Extract(*C, St);
            St->Kern(Tex->GetScaleFactor() * F->GetKerning(*C, *(C+1)));
            St->Draw();
            St->Advance();
        }
        ++C;
    }

    return BGE_SUCCESS;
}


bakge::Result PostRenderTest()
{
    St->Unbind();
    Tex->Unbind();
    UICam->Unbind();

    return BGE_SUCCESS;
}


bakge::Result ShutDownTest()
{
    if(Tex != NULL)
        delete Tex;

    if(UICam != NULL)
        delete UICam;

    if(F != NULL)
        delete F;

    delete St;

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
