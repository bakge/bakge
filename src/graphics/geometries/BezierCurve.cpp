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
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

// Log point/subdivision information when building LineStrip from BezierCurve
#define BGE_BEZIER_VERBOSE_BUILD 0
// Log curve points information when creating a BezierCurve
#define BGE_BEZIER_VERBOSE_CREATE 0
// Verbose output when manipulating anchor/control indices buffers
#define BGE_BEZIER_VERBOSE_BUFFER_OPERATIONS 0

namespace bakge
{

BezierCurve::BezierCurve()
{
    AllPoints = NULL;
    AnchorIndices = NULL;
    NumAnchors = 0;
}


BezierCurve::~BezierCurve()
{
    if(AllPoints != NULL)
        delete[] AllPoints;

    if(AnchorIndices != NULL)
        delete[] AnchorIndices;
}


BezierCurve* BezierCurve::Create(int NumPoints, Scalar* Points)
{
    try {
        BezierCurve* B = new BezierCurve;
        if(B == NULL)
            throw "Unable to allocate memory";

        B->NumPoints = NumPoints;
        // All BezierCurve objects start out amalgamated
        B->NumSegments = 1;
        // Amalgamated curves have 2 anchors; the rest are control points
        B->HighOrder = NumPoints - 2;

        glBindBuffer(GL_ARRAY_BUFFER, B->Buffers[GEOMETRY_BUFFER_POSITIONS]);
        glBufferData(GL_ARRAY_BUFFER, NumPoints * sizeof(Scalar) * 3, Points,
                                                            GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Allocate points cache
        B->AllPoints = new Vector3[NumPoints];
        // Indices buffer, for setting GL data store
        int* Indices = new int[NumPoints];

        // Start amalgamated; all but 2 endpoints are control points
        B->NumAnchors = 2;

        B->AnchorIndicesSize = 64;
        B->AnchorIndices = new int[B->AnchorIndicesSize];
        if(B->AnchorIndices == NULL) {
            Log("ERROR: BezierCurve::Create - Couldn't allocate anchor indices "
                                                                    "buffer.\n");
            delete B;
            return NULL;
        }

        B->AnchorIndices[0] = 0;
        B->AnchorIndices[1] = NumPoints - 1;

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_CREATE == 1
        BeginLogBlock();
        Log("BezierCurve anchor buffer size: %d\n", B->AnchorIndicesSize);
        Log("  Anchor points:\n");
        for(int i=0;i<B->NumAnchors;++i)
            Log("    %d: %d\n", i, B->AnchorIndices[i]);
        EndLogBlock();
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_CREATE == 1

        for(int i=0;i<NumPoints;++i) {
            Indices[i] = i;
            B->AllPoints[i][0] = Points[i * 3 + 0];
            B->AllPoints[i][1] = Points[i * 3 + 1];
            B->AllPoints[i][2] = Points[i * 3 + 2];
        }

        glBindBuffer(GL_ARRAY_BUFFER, B->Buffers[GEOMETRY_BUFFER_INDICES]);
        glBufferData(GL_ARRAY_BUFFER, NumPoints * sizeof(int), Indices,
                                                       GL_DYNAMIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        delete Indices;

        return B;
    } catch(const char* Message) {
        Log("ERROR: BezierCurve - %s\n", Message);
        return NULL;
    }
}


Result BezierCurve::Draw() const
{
    glDrawElements(GL_POINTS, NumPoints, GL_UNSIGNED_INT, (GLvoid*)0);

    return Geometry::Draw();
}


LineStrip* BezierCurve::Build(int NumSubdivisions)
{
    /* *
     * Each segment has 2 endpoints, but most segments share both points
     * with a neighboring segment. The number of segment "anchor" points
     * is NumSegments + 1.
     * */

    // 0 subdivisions is just a LineStrip of the control points.
    // If you need to draw this, just use the BezierCurve directly
    if(NumSubdivisions < 1) {
        Log("ERROR: BezierCurve - Creating a LineStrip requires > 0 "
                                                    "subdivisions.\n");
        return NULL;
    }

    // Anchor points = NumSegments + 1
    int NumLinePoints = NumSegments + 1;
    NumLinePoints += (NumSegments * NumSubdivisions);

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
    BeginLogBlock();
    Log("BezierCurve: Building LineStrip\n");
    Log("  %d Subdivisions.\n", NumSubdivisions);
    Log("  %d Points per segment.\n", NumSubdivisions + 1);
    Log("  %d Segments.\n", NumSegments);
    Log("  %d Total points.\n", NumLinePoints);
    EndLogBlock();
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1

    Vector3* CurvePoints = new Vector3[NumLinePoints];

    Scalar Advance = 1.0f / (NumSubdivisions + 1);

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
    // GetPointAt internal makes Log calls, put them in a block
    BeginLogBlock();
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1

    // Various metadata used for calculating points along the curve
    // Indices of anchors of segment currently being built
    int Start, End;
    // Number of control points in this particular segment.
    int NumControl;
    // Span of the segment (number points per segment)
    int Span;
    // Current offset into the CurvePoints buffer
    int Offset;
    for(int i=0;i<NumSegments;++i) {
        Start = AnchorIndices[i];
        End = AnchorIndices[i+1];
        // Anchor indices are sorted, letting us easily calculated # control
        // points in any given segment.
        NumControl = AnchorIndices[i+1] - AnchorIndices[i] - 1;
        Span = (NumSubdivisions + 1);
        Offset = i * Span;
        // Calculates all points between the anchors in a segment
        // X-----0-----0-----0-----X
        // Calculates the 0's
        for(int j=1;j<=NumSubdivisions;++j) {
            GetPointAt(NumControl, &AllPoints[Start], &CurvePoints[Offset+j],
                                                                Advance * j);
        }
        // Manually set anchor points in points buffer
        // X-----0-----0-----0-----X
        // Sets the X's to the anchor points values
        CurvePoints[Offset] = AllPoints[Start];
        CurvePoints[Offset + Span] = AllPoints[End];
    }

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
    EndLogBlock();
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1

    LineStrip* L = LineStrip::Create(NumLinePoints, &CurvePoints[0][0]);

    delete[] CurvePoints;

    return L;
}


Result BezierCurve::Amalgamate()
{
    return BGE_FAILURE;
}


int BezierCurve::Separate(int MinOrder)
{
    return -1;
}


void BezierCurve::GetPointAt(int NumControlPoints,
                        const Vector3* SegmentPoints,
                        Vector3* Points, Scalar T)
{
    // If there no control points, segment is a line. Simple case
    if(NumControlPoints < 1) {
        // Start at first anchor
        (*Points) = SegmentPoints[0];
        // Translate T * (anchor2 - anchor1)
        (*Points) += (SegmentPoints[1] - SegmentPoints[0]) * T;
#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
        Log("BezierCurve: Built point P(%2.2f) = (%2.3f, %2.3f, %2.3f)\n", T,
                                                        (*Points)[0],
                                                        (*Points)[1],
                                                        (*Points)[2]);
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
        return;
    }

#ifdef _DEBUG
    // Courtesy error-checking
    if(SegmentPoints == NULL) {
        Log("ERROR: BezierCurve - GetPointAt requires SegmentPoints "
                                                       "!= NULL.\n");
        return;
    }

    if(Buffers[GEOMETRY_BUFFER_POSITIONS] == NULL) {
        Log("ERROR: BezierCurve - GetPointAt requires Buffers[GEOMETRY_BUFFER_POSITIONS] "
                                                       "!= NULL.\n");
        return;
    }

    if(T < 0 || T > 1.0f) {
        Log("ERROR: BezierCurve - Cannot get point P(t) where t is < 0 "
                                                           "or > 1.\n");
        return;
    }
#endif // _DEBUG

    // Temporaries to store control points (we offset them)
    Vector3* Temp = new Vector3[NumControlPoints+2];

    // Intermediates start out at same position as anchors/control points
    for(int i=0;i<NumControlPoints+2;++i)
        Temp[i] = SegmentPoints[i];

    int Shifts = NumControlPoints + 1;

    // This loop shifts them to their intermediate positions.
    while(Shifts > 0) {
        for(int i=0;i<Shifts;++i)
            Temp[i] += (Temp[i+1] - Temp[i]) * T;
        --Shifts;
    }

    // Temp[0] will end up in the position P(T) after the final shift.
    (*Points) = Temp[0];

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1
    Log("BezierCurve: Built point P(%2.2f) = (%2.3f, %2.3f, %2.3f)\n", T,
                                                        (*Points)[0],
                                                        (*Points)[1],
                                                        (*Points)[2]);
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUILD == 1

    delete[] Temp;
}


bool BezierCurve::IsAnchor(int Index) const
{
    if(AnchorIndices == NULL)
        return false;

    if(NumAnchors == 0)
        return false;

    // Divide and conquer search, start in middle of array
    int Check = NumAnchors / 2;

    int Left = 0;
    int Right = NumAnchors - 1;

    while(1) {
        if(Index > AnchorIndices[Check]) {
            Left = Check + 1;
            if(Left > Right)
                break;
            Check = (Right + Left) / 2;
        } else if(Index < AnchorIndices[Check]) {
            Right = Check - 1;
            if(Left > Right)
                break;
            Check = (Right + Left) / 2;
        } else {
            return true;
        }
    }

    return false;
}


bool BezierCurve::IsControl(int Index) const
{
    return !IsAnchor(Index);
}


void BezierCurve::GetPointAt(Scalar T, Vector3* V)
{
}


int BezierCurve::MakeAnchor(int PointIndex)
{
    if(PointIndex >= NumPoints) {
        Log("ERROR: BezierCurve - Index %d is out of bounds.\n", PointIndex);
        return -1;
    }

    if(AnchorIndices == NULL) {
        Log("ERROR: BezierCurve - Anchor indices buffer is NULL.\n");
        return -1;
    }

    // Do a binary search to find where the new index belongs
    int Check = NumAnchors / 2;
    int Left = 0;
    int Right = NumAnchors - 1;
    int Index;

    while(1) {
        if(PointIndex > AnchorIndices[Check]) {
            Left = Check + 1;
            if(Left > Right)
                break;
            Check = (Right + Left) / 2;
        } else if(PointIndex < AnchorIndices[Check]) {
            Right = Check - 1;
            if(Left > Right)
                break;
            Check = (Right + Left) / 2;
        } else {
            // Already an anchor point
            Log("ERROR: BezierCurve - Point at index %d is already an "
                                                "anchor.\n", PointIndex);
            return 0;
        }
    }

    Index = Left;

    // Adding an anchor splits some segment into two.
    ++NumSegments;

    // Making an anchor removes a control point
    --NumControlPoints;
    ++NumAnchors;

    // Reallocate the buffer if it is not large enough to hold all indices
    if(NumAnchors > AnchorIndicesSize) {
        int OldSize = AnchorIndicesSize;
        int* OldArray = AnchorIndices;
        AnchorIndicesSize *= 2;
        AnchorIndices = new int[AnchorIndicesSize];
        memcpy((void*)AnchorIndices, (const void*)OldArray, OldSize * 4);
        BeginLogBlock();
        Log("BezierCurve - Reallocating anchor indices buffer.\n");
        Log("  - Old size: %d\n", OldSize);
        Log("  - New size: %d\n", AnchorIndicesSize);
        EndLogBlock();
    }

    // Shift data to make room for the new index.
    for(int i=NumAnchors;i>=Index;--i)
        AnchorIndices[i] = AnchorIndices[i-1];

    AnchorIndices[Index] = PointIndex;

#if defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUFFER_OPERATIONS == 1
    BeginLogBlock();
    Log("BezierCurve - Anchors list\n");
    for(int i=0;i<NumAnchors;++i)
        Log("  %d: %d\n", i, AnchorIndices[i]);
    EndLogBlock();
#endif // defined(_DEBUG) && BGE_BEZIER_VERBOSE_BUFFER_OPERATIONS == 1

    return 1;
}


int BezierCurve::MakeControl(int PointIndex)
{
    if(IsControl(PointIndex))
        return 0;

    return 1;
}

} /* bakge */
