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
#include <bakge/internal/Format.h>
#ifdef _DEBUG
#include <bakge/internal/Debug.h>
#endif // _DEBUG

namespace bakge
{

Mesh::Mesh()
{
}


Mesh::~Mesh()
{
}


Mesh* Mesh::Create()
{
    Mesh* M = new Mesh;
    if(M == NULL) {
        Log("ERROR: Mesh - Couldn't allocate memory.\n");
        return NULL;
    }

    if(M->CreateBuffers() == BGE_FAILURE) {
        Log("ERROR: Mesh - Error creating mesh buffers.\n");
        delete M;
        return NULL;
    }

    return M;
}


Result Mesh::Encode100(const char* Path)
{
    BeginLogBlock();

    PHYSFS_file* F = PHYSFS_openWrite(Path);
    if(F == NULL) {
        Log("ERROR: Mesh::Encode100() - Unable to open \"%s\" for writing.\n",
                                                                        Path);
        EndLogBlock();
        return BGE_FAILURE;
    }

    bmf::layout100::Header H;
    strcpy((char*)(&H.FormatName), BGE_BMF_NAME);
    H.Type = 0;
    H.Major = 1;
    H.Minor = 0;
    H.Revision = 0;

    if(PHYSFS_write(F, (void*)&H, sizeof(H), 1) < 1) {
        Log("ERROR: Mesh::Encode100() - Error writing file header.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    int Num = GetNumVertices();

    if(PHYSFS_write(F, (void*)&Num, sizeof(Num), 1) < 1) {
        Log("ERROR: Mesh::Encode100() - Error writing vertex metadatum.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    if(PHYSFS_write(F, (void*)GetPositionData(), sizeof(Scalar) * 3,
                                                         Num) < Num)
    {
        Log("ERROR: Mesh::Encode100() - Error writing vertex position data "
                                                                "segment.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    if(PHYSFS_write(F, (void*)GetNormalData(), sizeof(Scalar) * 3,
                                                       Num) < Num)
    {
        Log("ERROR: Mesh::Encode100() - Error writing vertex normals data "
                                                                "segment.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    if(PHYSFS_write(F, (void*)GetTexCoordData(), sizeof(Scalar) * 2,
		                                        Num) < Num)
    {
        Log("ERROR: Mesh::Encode100() - Error writing vertex texcoords data "
                                                                "segment.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    Num = GetNumIndices();

    if(PHYSFS_write(F, (void*)&Num, sizeof(Num), 1) < 1) {
        Log("ERROR: Mesh::Encode100() - Error writing triangles metadatum.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    if(PHYSFS_write(F, (void*)GetIndexData(), sizeof(int), Num) < Num) {
        Log("ERROR: Mesh::Encode100() - Error writing triangle indices data "
                                                                "segment.\n");
        PHYSFS_close(F);
        EndLogBlock();
        return BGE_FAILURE;
    }

    PHYSFS_close(F);

    EndLogBlock();

    return BGE_SUCCESS;
}


Mesh* Mesh::Decode100(const char* Path)
{
    bmf::v100* Handle = OpenMeshFile100(Path);
    if(Handle == NULL)
        return NULL;

    Mesh* M = new Mesh;
    if(M == NULL)
        return NULL;

    BeginLogBlock();
    Log("Mesh::Decode100 - \"%s\"\n", Path);

    uint32 VertCount;
    if(bakge::GetNumVertices(Handle, &VertCount) == BGE_FAILURE) {
        Log("  ERROR: Unable to get number of vertices.\n");
	delete M;
	EndLogBlock();
        return NULL;
    }

    Scalar* P = (Scalar*)malloc(sizeof(Scalar) * 3 * VertCount);
    if(P == NULL) {
        Log("  ERROR: Couldn't allocate positions cache buffer.\n");
	delete M;
	EndLogBlock();
        return NULL;
    }

    Log("  - Allocated positions cache buffer.\n");

    Scalar* N = (Scalar*)malloc(sizeof(Scalar) * 3 * VertCount);
    if(N == NULL) {
        Log("  ERROR: Couldn't allocate normals cache buffer.\n");
	free(P);
	delete M;
	EndLogBlock();
        return NULL;
    }

    Log("  - Allocated normals cache buffer.\n");

    Scalar* T = (Scalar*)malloc(sizeof(Scalar) * 2 * VertCount);
    if(T == NULL) {
        Log("  ERROR: Couldn't allocate texcoords cache buffer.\n");
	free(P);
	free(N);
	delete M;
	EndLogBlock();
        return NULL;
    }

    Log("  - Allocated texcoords cache buffer.\n");

    uint32 IndCount;
    if(bakge::GetNumIndices(Handle, &IndCount) == BGE_FAILURE) {
        Log("  ERROR: Unable to get number of indices.\n");
	free(P);
	free(N);
	free(T);
	delete M;
	EndLogBlock();
        return NULL;
    }

    uint32* I = (uint32*)malloc(sizeof(uint32) * 3 * IndCount);
    if(I == NULL) {
        Log("  ERROR: Couldn't allocate indices cache buffer.\n");
	free(P);
	free(N);
	free(T);
	delete M;
	EndLogBlock();
        return NULL;
    }

    Log("  - Allocated indices cache buffer.\n");

    // Extract data from file
    GetVertexPositions(Handle, P);
    GetVertexNormals(Handle, N);
    GetVertexTexCoords(Handle, T);
    GetTriangleIndices(Handle, I);

    Log("  - Setting mesh buffer pointers and metadata.\n");

    // Set mesh's cache buffer pointers and metadata
    M->Positions = P;
    M->Normals = N;
    M->TexCoords = T;
    M->Indices = I;
    M->NumIndices = IndCount;
    M->NumVertices = VertCount;

    if(M->CreateBuffers() == BGE_FAILURE) {
        Log("  ERROR: Unable to create GL buffers.\n");
	free(P);
	free(N);
	free(T);
	delete M;
	EndLogBlock();
        return NULL;
    }

    // Fill data store data using cache buffers
    glBindBuffer(GL_ARRAY_BUFFER, M->ShapeBuffers[SHAPE_BUFFER_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 3 * VertCount,
                                (const GLvoid*)P, GL_STATIC_DRAW);
    Log("  - Filled GL positions buffer data store.\n");

    glBindBuffer(GL_ARRAY_BUFFER, M->ShapeBuffers[SHAPE_BUFFER_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 3 * VertCount,
                                (const GLvoid*)N, GL_STATIC_DRAW);
    Log("  - Filled GL normals buffer data store.\n");

    glBindBuffer(GL_ARRAY_BUFFER, M->ShapeBuffers[SHAPE_BUFFER_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * 2 * VertCount,
                                (const GLvoid*)T, GL_STATIC_DRAW);
    Log("  - Filled GL texcoords buffer data store.\n");

    glBindBuffer(GL_ARRAY_BUFFER, M->ShapeBuffers[SHAPE_BUFFER_INDICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * IndCount, (const GLvoid*)N,
                                                            GL_STATIC_DRAW);
    Log("  - Filled GL indices buffer data store.\n");

    EndLogBlock();

    return M;
}


Result Mesh::CreateBuffers()
{
    /* If data already exists clear it */
    ClearBuffers();

    glGenBuffers(NUM_SHAPE_BUFFERS, ShapeBuffers);

#ifdef _DEBUG
    /* Check to make sure each of our mesh's buffers was created properly */
    if(ShapeBuffers[SHAPE_BUFFER_POSITIONS] == 0) {
        Log("ERROR: Mesh - Couldn't create positions buffer\n");
        return BGE_FAILURE;
    }

    if(ShapeBuffers[SHAPE_BUFFER_NORMALS] == 0) {
        Log("ERROR: Mesh - Couldn't create normals buffer\n");
        return BGE_FAILURE;
    }

    if(ShapeBuffers[SHAPE_BUFFER_TEXCOORDS] == 0) {
        Log("ERROR: Mesh - Couldn't create texture coordinates buffer\n");
        return BGE_FAILURE;
    }

    if(ShapeBuffers[SHAPE_BUFFER_INDICES] == 0) {
        Log("ERROR: Mesh - Couldn't create indices buffer\n");
        return BGE_FAILURE;
    }
#endif /* _DEBUG */

    return BGE_SUCCESS;
}


Result Mesh::ClearBuffers()
{
    if(ShapeBuffers[0] != 0) {
        glDeleteBuffers(NUM_SHAPE_BUFFERS, ShapeBuffers);
        memset((void*)ShapeBuffers, 0, sizeof(GLuint) * NUM_SHAPE_BUFFERS);
    }

    return BGE_SUCCESS;
}


Result Mesh::SetPositionData(int NumPositions, const Scalar* Data)
{
    if(ShapeBuffers[SHAPE_BUFFER_POSITIONS] == 0) {
        Log("ERROR: Mesh::SetPositionData - Positions buffer doesn't "
                                                            "exist.\n");
        return BGE_FAILURE;
    }

    NumVertices = NumPositions;

    BeginLogBlock();
    Log("Mesh::SetPositionData\n");

    if(Positions != NULL) {
        Log("  - Freeing old positions cache...\n");
        free(Positions);
    }

    size_t Size = sizeof(Scalar) * 3 * NumPositions;

    Positions = (Scalar*)malloc(Size);
    if(Positions == NULL) {
        Log("  ERROR: Couldn't allocate new cache (size 0x%08x)\n", Size);
        EndLogBlock();
        return BGE_FAILURE;
    }

    Log("  - Allocated new cache buffer (size 0x%08x)\n", Size);

    memcpy((void*)Positions, (const void*)Data, Size);

    Log("  - Copied new buffer data to cache.\n");

    glBindBuffer(GL_ARRAY_BUFFER, ShapeBuffers[SHAPE_BUFFER_POSITIONS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * NumPositions * 3,
                                            (const GLvoid*)Data,
                                                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Log("  - Filled GL buffer data store with new data.\n");

    EndLogBlock();

    return BGE_SUCCESS;
}


Result Mesh::SetNormalData(int NumNormals, const Scalar* Data)
{
    if(ShapeBuffers[SHAPE_BUFFER_NORMALS] == 0) {
        Log("ERROR: Mesh::SetNormalData - Normals buffer doesn't exist.\n");
        return BGE_FAILURE;
    }

    BeginLogBlock();
    Log("Mesh::SetNormalData\n");

    if(Normals != NULL) {
        Log("  - Freeing old normals cache...\n");
        free(Normals);
    }

    size_t Size = sizeof(Scalar) * 3 * NumNormals;

    Normals = (Scalar*)malloc(Size);
    if(Normals == NULL) {
        Log("  ERROR: Couldn't allocate new cache (size 0x%08x)\n", Size);
        EndLogBlock();
        return BGE_FAILURE;
    }

    Log("  - Allocated new cache buffer (size 0x%08x)\n", Size);

    memcpy((void*)Normals, (const void*)Data, Size);

    Log("  - Copied new buffer data to cache.\n");

    glBindBuffer(GL_ARRAY_BUFFER, ShapeBuffers[SHAPE_BUFFER_NORMALS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * NumNormals * 3,
                                            (const GLvoid*)Data,
                                                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Log("  - Filled GL buffer data store with new data.\n");

    EndLogBlock();

    return BGE_SUCCESS;
}


Result Mesh::SetIndexData(int NumIndices, const uint32* Data)
{
    if(ShapeBuffers[SHAPE_BUFFER_INDICES] == 0) {
        Log("ERROR: Mesh::SetIndexData - Indices buffer doesn't exist.\n");
        return BGE_FAILURE;
    }

    BeginLogBlock();
    Log("Mesh::SetIndexData\n");

    if(Indices != NULL) {
        Log("  - Freeing old indices cache...\n");
        free(Indices);
    }

    this->NumIndices = NumIndices;

    size_t Size = sizeof(uint32) * 3 * NumIndices;

    Indices = (uint32*)malloc(Size);
    if(Indices == NULL) {
        Log("  ERROR: Couldn't allocate new cache (size 0x%08x)\n", Size);
        EndLogBlock();
        return BGE_FAILURE;
    }

    Log("  - Allocated new cache buffer (size 0x%08x)\n", Size);

    memcpy((void*)Indices, (const void*)Data, Size);

    Log("  - Copied new buffer data to cache.\n");

    glBindBuffer(GL_ARRAY_BUFFER, ShapeBuffers[SHAPE_BUFFER_INDICES]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uint32) * NumIndices,
                                            (const GLvoid*)Data,
                                                GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Log("  - Filled GL buffer data store with new data.\n");

    EndLogBlock();

    return BGE_SUCCESS;
}


Result Mesh::SetTexCoordData(int NumTexCoords, const Scalar* Data)
{
    if(ShapeBuffers[SHAPE_BUFFER_TEXCOORDS] == 0) {
        Log("ERROR: Mesh::SetTexCoordData - Texcoords buffer doesn't "
                                                            "exist.\n");
        return BGE_FAILURE;
    }

    BeginLogBlock();
    Log("Mesh::SetTexCoordData\n");

    if(TexCoords != NULL) {
        Log("  - Freeing old texcoords cache...\n");
        free(TexCoords);
    }

    size_t Size = sizeof(Scalar) * 2 * NumTexCoords;

    TexCoords = (Scalar*)malloc(Size);
    if(TexCoords == NULL) {
        Log("  ERROR: Couldn't allocate new cache (size 0x%08x)\n", Size);
        EndLogBlock();
        return BGE_FAILURE;
    }

    Log("  - Allocated new cache buffer (size 0x%08x)\n", Size);

    memcpy((void*)TexCoords, (const void*)Data, Size);

    Log("  - Copied new buffer data to cache.\n");

    glBindBuffer(GL_ARRAY_BUFFER, ShapeBuffers[SHAPE_BUFFER_TEXCOORDS]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Scalar) * NumTexCoords * 2,
                                                (const GLvoid*)Data,
                                                    GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    Log("  - Filled GL buffer data store with new data.\n");

    EndLogBlock();

    return BGE_SUCCESS;
}

} /* bakge */
