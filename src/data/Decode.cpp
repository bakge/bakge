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

namespace bakge
{

namespace bmf
{

struct v100
{
    char* Path;
    PHYSFS_file* F;
    // Cache number of vertices and triangles in the mesh
    uint32 NumVertices;
    uint32 NumIndices;
    // File offsets of various vertex data
    uint64 PositionsOffset;
    uint64 NormalsOffset;
    uint64 TexCoordsOffset;
    uint64 TrianglesOffset;
};

} // bmf

// Called by stb_image when it requires data to process
static int _stbicb_Read(void* User, char* Data, int Size)
{
    return (int)PHYSFS_read((PHYSFS_File*)User, (void*)Data, 1, Size);
}

static void _stbicb_Skip(void* User, uint32 Offset)
{
    uint32 Current = (uint32)PHYSFS_tell((PHYSFS_File*)User);
    PHYSFS_seek((PHYSFS_File*)User, Current + Offset);
}

static int _stbicb_EOF(void* User)
{
    return PHYSFS_eof((PHYSFS_File*)User);
}

Result DecodeImageFile(const char* FilePath, Byte** Data, int* W,
                                                int* H, int* N)
{
    BeginLogBlock();

    Log("DecodeImageFile - \"%s\"\n", FilePath);

    if(PHYSFS_exists(FilePath) == 0) {
        Log("  ERROR: File does not exist.\n");
        return BGE_FAILURE;
    }

    PHYSFS_File* F = PHYSFS_openRead(FilePath);
    if(F == NULL) {
        Log("  ERROR: Couldn't open file for reading.\n");
        return BGE_FAILURE;
    }

    stbi_io_callbacks Callbacks;
    Callbacks.read = _stbicb_Read;
    Callbacks.skip = _stbicb_Skip;
    Callbacks.eof = _stbicb_EOF;

    *Data = stbi_load_from_callbacks(&Callbacks, (void*)F, W, H, N, 4);

    BeginLogBlock();

    Result R = BGE_SUCCESS;

    if(*Data != NULL) {
        Log("  - \"%s\" successfully decoded.\n", FilePath);
        Log("  - %dx%d, %d bytes per pixel\n", *W, *H, *N);
    } else {
        Log("  ERROR: Error while decoding \"%s\".\n", FilePath);
        R = BGE_FAILURE;
    }

    PHYSFS_close(F);

    Log("  - Closing file \"%s\"\n", FilePath);

    EndLogBlock();

    return R;
}


bmf::v100* OpenMeshFile100(const char* Path)
{
    BeginLogBlock();

    Log("OpenMeshFile100() - \"%s\"\n", Path);

    if(PHYSFS_exists(Path) == 0) {
        Log("ERROR: OpenMeshFile100() - File \"%s\" does not exist.\n",
                                                                Path);
        EndLogBlock();
        return NULL;
    }

    if(PHYSFS_isDirectory(Path) != 0) {
        Log("ERROR: OpenMeshFile100() - \"%s\" is a directory.\n", Path);
        EndLogBlock();
        return NULL;
    }

    PHYSFS_file* MeshFile = PHYSFS_openRead(Path);
    if(MeshFile == NULL) {
        Log("ERROR: OpenMeshFile100() - Error while opening file \"%s\".\n",
                                                                      Path);
        EndLogBlock();
        return NULL;
    }

    bmf::v100* Handle = new bmf::v100;
    if(Handle == NULL) {
        Log("ERROR: OpenMeshFile100() - Couldn't allocate memory.\n");
        PHYSFS_close(MeshFile);
        EndLogBlock();
        return NULL;
    }

    Log("  - Verifying format and version...\n", Path);

    bmf::layout100::Header H;

    if(PHYSFS_read(MeshFile, (void*)&H, sizeof(H), 1) < 1) {
        Log("ERROR: OpenMeshFile100() - Couldn't find file header.\n");
        PHYSFS_close(MeshFile);
        EndLogBlock();
        return NULL;
    }

    // Make sure name string is correct
    if(strncmp(BGE_BMF_NAME, H.FormatName, 32) != 0) {
        Log("ERROR: OpenMeshFile100() - \"%s\" is not a Bakge Mesh File.\n",
                                                                      Path);
        PHYSFS_close(MeshFile);
        EndLogBlock();
        return NULL;
    }

   if(H.Major != 1 || H.Minor != 0 || H.Revision != 0) {
       Log("ERROR: OpenMeshFile100() - \"%s\" is of an incorrect format "
                                                "version (v%d.%d.%d).\n",
                                                    Path, H.Major, H.Minor,
                                                            H.Revision);
        PHYSFS_close(MeshFile);
        EndLogBlock();
        return NULL;
   }

    Log("  - Verified Bakge Mesh File v%d.%d.%d \"%s\"\n", H.Major, H.Minor,
                                                            H.Revision, Path);

    PHYSFS_uint64 Offset = sizeof(bmf::layout100::Header);

    PHYSFS_sint64 ReadObj = PHYSFS_read(MeshFile,
                    (void*)(&Handle->NumVertices),
                               sizeof(uint32), 1);
    if(ReadObj < 1) {
        Log("ERROR: OpenMeshFile100() - Error reading vertex metadatum.\n");
        PHYSFS_close(MeshFile);
        delete Handle;
        EndLogBlock();
        return NULL;
    }

    Log("  - Found vertex metadatum at offset 0x%x\n", Offset);
    Log("  - Number of vertices: %d\n", Handle->NumVertices);

    // Reading advances file offset
    Offset += sizeof(uint32);

    Log("  - Setting vertex data segment offsets...\n");

    Handle->PositionsOffset = Offset;
    Log("    - Vertex positions at offset  0x%0x\n", Offset);
    // Advance past positions
    Offset += sizeof(Scalar) * 3 * Handle->NumVertices;

    Handle->NormalsOffset = Offset;
    Log("    - Vertex normals at offset    0x%0x\n", Offset);
    // Advance past normals
    Offset += sizeof(Scalar) * 3 * Handle->NumVertices;

    Handle->TexCoordsOffset = Offset;
    Log("    - Vertex texcoords at offset  0x%0x\n", Offset);
    // Advance past texcoords
    Offset += sizeof(Scalar) * 2 * Handle->NumVertices;

    if(PHYSFS_seek(MeshFile, Offset) == 0) {
        Log("ERROR: OpenMeshFile100() - Error seeking triangle metadatum.\n");
        PHYSFS_close(MeshFile);
        delete Handle;
        EndLogBlock();
        return NULL;
    }

    ReadObj = PHYSFS_read(MeshFile, (void*)(&Handle->NumIndices),
                                                sizeof(uint32), 1);
    if(ReadObj < 1) {
        Log("ERROR: OpenMeshFile100() - Error reading triangle metadatum.\n");
        PHYSFS_close(MeshFile);
        delete Handle;
        EndLogBlock();
        return NULL;
    }

    Log("  - Found triangle metadatum at offset 0x%x\n", Offset);
    Log("  - Number of indices: %d\n", Handle->NumIndices);

    Offset += sizeof(uint32);

    Log("  - Setting triangle data segment offset...\n");

    Handle->TrianglesOffset = Offset;
    Log("    - Triangle indices at offset  0x%0x\n", Offset);

    Log("  - Successfully opened and scanned mesh file \"%s\".\n", Path);
    Log("\n");

    EndLogBlock();

    Handle->F = MeshFile;

#ifdef _MSC_VER
    Handle->Path = _strdup(Path);
#else
    Handle->Path = strdup(Path);
#endif // _MSC_VER

    return Handle;
}


Result CloseMeshFile100(bmf::v100* Handle)
{
    if(PHYSFS_close(Handle->F) == 0) {
        Log("ERROR: CloseMeshFile100() - Error closing mesh file handle.\n");
        Log("  - %s\n", PHYSFS_getLastError());
        return BGE_FAILURE;
    }

    Log("Successfully closed mesh file \"%s\".\n", Handle->Path);

    free(Handle->Path);

    return BGE_SUCCESS;
}


Result GetNumVertices(bmf::v100* Handle, uint32* Num)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetNumVertices() - Passed NULL bmf::v100*\n");
        return BGE_FAILURE;
    }

    if(Num == NULL) {
        Log("ERROR: GetNumVertices() - Passed NULL uint32*\n");
        return BGE_FAILURE;
    }
#endif // _DEBUG

    *Num = Handle->NumVertices;

    return BGE_SUCCESS;
}


Result GetNumIndices(bmf::v100* Handle, uint32* Num)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetNumIndices() - Passed NULL bmf::v100*\n");
        return BGE_FAILURE;
    }

    if(Num == NULL) {
        Log("ERROR: GetNumIndices() - Passed NULL uint32*\n");
        return BGE_FAILURE;
    }
#endif // _DEBUG

    *Num = Handle->NumIndices;

    return BGE_SUCCESS;
}


void GetVertexPositions(bmf::v100* Handle, Scalar* Buffer)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetVertexPositions() - Passed NULL bmf::v100*\n");
        return;
    }

    if(Buffer == NULL) {
        Log("ERROR: GetVertexPositions() - Passed NULL Scalar*\n");
        return;
    }
#endif // _DEBUG

    if(PHYSFS_seek(Handle->F, Handle->PositionsOffset) == 0) {
        Log("ERROR: GetVertexPositions (v100) - Error seeking positions "
                                                            "offset.\n");
        return;
    }

    uint32 Count;
    GetNumVertices(Handle, &Count);
    Count *= 3 * sizeof(Scalar);

    PHYSFS_sint64 Read = PHYSFS_read(Handle->F, Buffer, 1, Count);
    if(Read < (PHYSFS_sint64)Count) {
        Log("ERROR: GetVertexPositions (v100) - Incomplete read from "
                                                            "file.\n");
        Log("  %d of %d read\n", Read, Count);
        return;
    }
}


void GetVertexNormals(bmf::v100* Handle, Scalar* Buffer)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetVertexNormals() - Passed NULL bmf::v100*\n");
        return;
    }

    if(Buffer == NULL) {
        Log("ERROR: GetVertexNormals() - Passed NULL Scalar*\n");
        return;
    }
#endif // _DEBUG

    if(PHYSFS_seek(Handle->F, Handle->NormalsOffset) == 0) {
        Log("ERROR: GetVertexNormals (v100) - Error seeking normals "
                                                            "offset.\n");
        return;
    }

    uint32 Count;
    GetNumVertices(Handle, &Count);
    Count *= 3 * sizeof(Scalar);

    PHYSFS_sint64 Read = PHYSFS_read(Handle->F, Buffer, 1, Count);
    if(Read < (PHYSFS_sint64)Count) {
        Log("ERROR: GetVertexNormals (v100) - Incomplete read from "
                                                            "file.\n");
        Log("  %d of %d read\n", Read, Count);
        return;
    }
}


void GetVertexTexCoords(bmf::v100* Handle, Scalar* Buffer)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetVertexTexCoords() - Passed NULL bmf::v100*\n");
        return;
    }

    if(Buffer == NULL) {
        Log("ERROR: GetVertexTexCoords() - Passed NULL Scalar*\n");
        return;
    }
#endif // _DEBUG

    if(PHYSFS_seek(Handle->F, Handle->TexCoordsOffset) == 0) {
        Log("ERROR: GetVertexTexCoords (v100) - Error seeking texcoords "
                                                            "offset.\n");
        return;
    }

    uint32 Count;
    GetNumVertices(Handle, &Count);
    Count *= 2 * sizeof(Scalar);

    PHYSFS_sint64 Read = PHYSFS_read(Handle->F, Buffer, 1, Count);
    if(Read < (PHYSFS_sint64)Count) {
        Log("ERROR: GetVertexTexCoords (v100) - Incomplete read from "
                                                            "file.\n");
        Log("  %d of %d read\n", Read, Count);
        return;
    }
}


void GetTriangleIndices(bmf::v100* Handle, uint32* Buffer)
{
#ifdef _DEBUG
    if(Handle == NULL) {
        Log("ERROR: GetTriangleIndices() - Passed NULL bmf::v100*\n");
        return;
    }

    if(Buffer == NULL) {
        Log("ERROR: GetTriangleIndices() - Passed NULL uint32*\n");
        return;
    }
#endif // _DEBUG

    if(PHYSFS_seek(Handle->F, Handle->TrianglesOffset) == 0) {
        Log("ERROR: GetTriangleIndices (v100) - Error seeking indices "
                                                            "offset.\n");
        return;
    }

    uint32 Count;
    GetNumIndices(Handle, &Count);
    Count *= sizeof(uint32);

    PHYSFS_sint64 Read = PHYSFS_read(Handle->F, Buffer, 1, Count);
    if(Read < (PHYSFS_sint64)Count) {
        Log("ERROR: GetTriangleIndices (v100) - Incomplete read from "
                                                            "file.\n");
        Log("  %d of %d read\n", Read, Count);
        return;
    }
}

} /* bakge */
