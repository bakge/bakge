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
#include <bakge/internal/Graphics.h>
#include <bakge/internal/Error.h>
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

Grid::Grid()
{
    UnitWidth = 1.0f;
    UnitLength = 1.0f;
    HalfRows = 3;
    HalfCols = 3;
}


Grid::~Grid()
{
}


Grid* Grid::Create(int HalfRows, int HalfCols, Scalar Width, Scalar Length)
{
    if(HalfRows < 1) {
        Log("ERROR: Grid - Requires HalfRows > 0.\n");
        return NULL;
    }

    if(HalfCols < 1) {
        Log("ERROR: Grid - Requires HalfCols > 0.\n");
        return NULL;
    }

    Grid* G;

    try {
        G = (Grid*)calloc(1, sizeof(Grid));
        if(G == NULL)
            throw BGE_FACTORY_MESSAGE_BADALLOC;

        new(G) Grid;

        G->HalfRows = HalfRows;
        G->HalfCols = HalfCols;
        G->UnitWidth = Width;
        G->UnitLength = Length;
        G->NumPoints = 2 * ((HalfCols * 2 + 1) + (HalfRows * 2 + 1));

        if(G->Bufferize() != BGE_SUCCESS) {
            Log("ERROR: Grid::Create - Failed to bufferize the grid.\n");
            delete G;
            return NULL;
        }

        return G;
    } catch(const char* Message) {
        Log("ERROR: Grid - %s\n", Message);
        free(G);
        return NULL;
    }
}


Result Grid::Bufferize()
{
    int Tries = 0;
    int NumColLines = (HalfCols * 2 + 1);
    int NumRowLines = (HalfRows * 2 + 1);

    Log("Grid - Bufferizing...\n");
    Log("  %d points.\n", NumPoints);
    Log("  %d lines.\n", NumColLines + NumRowLines);

    do {
        glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(GEOMETRY_BUFFER_POSITIONS));
        glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 3 * NumPoints, NULL,
                                                            GL_DYNAMIC_DRAW);

        Scalar* BufPtr = (Scalar*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if(BufPtr == NULL) {
            Log("ERROR: Grid - Failed to map buffer "
                            "(attempt %d).\n", ++Tries);

            // Sentinel to avoid infinite or long loops
            if(Tries >= BGE_MAP_BUFFER_MAX_ATTEMPTS) {
                Log("ERROR: Grid - Couldn't map buffer after %d attempts.\n",
                                               BGE_MAP_BUFFER_MAX_ATTEMPTS);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                return BGE_FAILURE;
            }

            continue;
        }

        int At = 0;

        for(int i=-HalfCols;i<=HalfCols;++i) {
            BufPtr[At++] = i * UnitWidth;
            BufPtr[At++] = 0;
            BufPtr[At++] = HalfRows * UnitLength;
            BufPtr[At++] = i * UnitWidth;
            BufPtr[At++] = 0;
            BufPtr[At++] = -HalfRows * UnitLength;
        }

        for(int i=-HalfRows;i<=HalfRows;++i) {
            BufPtr[At++] = -HalfCols * UnitWidth;
            BufPtr[At++] = 0;
            BufPtr[At++] = i * UnitLength;
            BufPtr[At++] = HalfCols * UnitWidth;
            BufPtr[At++] = 0;
            BufPtr[At++] = i * UnitLength;
        }

    } while(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE);

    Tries = 0;

    do {
        glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(GEOMETRY_BUFFER_INDICES));
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLuint) * NumPoints * 2, NULL,
                                                            GL_DYNAMIC_DRAW);

        int* BufPtr = (int*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if(BufPtr == NULL) {
            Log("ERROR: Grid - Failed to map indices buffer "
                                "(attempt %d).\n", ++Tries);

            // Sentinel to avoid infinite or long loops
            if(Tries >= BGE_MAP_BUFFER_MAX_ATTEMPTS) {
                Log("ERROR: Grid - Couldn't map buffer after %d attempts.\n",
                                               BGE_MAP_BUFFER_MAX_ATTEMPTS);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                return BGE_FAILURE;
            }

            continue;
        }

        for(int i=0;i < NumPoints;++i) {
            BufPtr[i] = i;
        }

    } while(glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE);

    return BGE_SUCCESS;
}


Result Grid::SetDrawStyle(GEOMETRY_DRAW_STYLE Style)
{
    if(Style == GEOMETRY_DRAW_STYLE_LINES) {
        DrawStyle = GL_LINES;
        return BGE_SUCCESS;
    } else {
        return Geometry::SetDrawStyle(Style);
    }
}


int Grid::SetHalfRows(int R)
{
    if(HalfRows < 1) {
        Log("ERROR: Grid - Requires HalfRows > 0.\n");
        return -1;
    }

    HalfRows = R;

    NumPoints = 2 * ((HalfCols * 2 + 1) + (HalfRows * 2 + 1));

    if(Bufferize() == BGE_FAILURE) {
        Log("ERROR: Grid - Failed to bufferize with new half-rows.\n");
        return -1;
    }

    return HalfRows;
}


int Grid::SetHalfCols(int C)
{
    if(HalfCols < 1) {
        Log("ERROR: Grid - Requires HalfCols > 0.\n");
        return -1;
    }

    HalfCols = C;

    NumPoints = 2 * ((HalfCols * 2 + 1) + (HalfRows * 2 + 1));

    if(Bufferize() == BGE_FAILURE) {
        Log("ERROR: Grid - Failed to bufferize with new half-columns.\n");
        return -1;
    }

    return HalfCols;
}


int Grid::GetHalfRows() const
{
    return HalfRows;
}


int Grid::GetHalfCols() const
{
    return HalfCols;
}

} // bakge
