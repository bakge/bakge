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

#ifndef BAKGE_BAKGE_H
#define BAKGE_BAKGE_H

/* Include standard library dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

/* SSE and SSE2 instructions headers */
#include <mmintrin.h>
#include <xmmintrin.h>
#include <emmintrin.h>

/* GCC & Clang attributes */
#if defined __GNUC__ || defined __clang__ || defined __MINGW__
/* Send compiler warning if function return result is not used */
#define BGE_WUNUSED __attribute__((warn_unused_result))
/* Mark a variable as possibly unused in its function  */
#define BGE_UNUSED __attribute__((unused))
/* Factory functions are static class BGE_API methods that return allocated memory  */
#else /* Define them anyways to avoid compilation errors  */
#define BGE_WUNUSED
#define BGE_UNUSED
#endif /* __GNUC__  */

/* MSVC pragmas */
#ifdef _MSC_VER
/* Prevent library conflict warning */
#ifdef _DEBUG
#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#endif /* _DEBUG */
#ifdef BAKGE_EXPORTS
#define BGE_API __declspec(dllexport)
#else
#ifndef BAKGE_STATIC
#define BGE_API __declspec(dllimport)
#else
#define GLEW_STATIC
#define BGE_API
#endif /* BAKGE_STATIC */
#endif /* BAKGE_EXPORTS */
#else
#define BGE_API
#ifndef BAKGE_EXPORTS
#define GLEW_STATIC
#endif /* BAKGE_EXPORTS */
#endif /* _MSC_VER */

/* Auxiliary definitions */
#ifdef _MSC_VER
#define BGE_ALIGN(X) __declspec(align(X))
#else
#define BGE_ALIGN(X) __attribute__((aligned(X)))
#endif

/* Common definitions */
#define BGE_FUNC extern BGE_API
#define BGE_FACTORY static BGE_WUNUSED
#define BGE_NCP const&
#define BGE_INL inline
#define BGE_VER_MAJ 0
#define BGE_VER_MIN 0
#define BGE_VER_REV 0

/* Required extensions to run Bakge */
#define VERTEX_BUFFER_OBJECT_EXT "GL_ARB_vertex_buffer_object"

/* Include external library headers */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
extern "C"
{
#include <lua/lua.h>
#include <lua/luaconf.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}
#include <physfs/physfs.h>

/* Include other external libraries */
#define STBI_HEADER_FILE_ONLY
#include <stb/stb_image.c>
#undef STBI_HEADER_FILE_ONLY
#include <stb/stb_truetype.h>
#define STB_TRUETYPE_IMPLEMENTATION

/* Include core Bakge classes and headers */
#include <bakge/core/Type.h>
#include <bakge/core/Input.h>
#include <bakge/core/Utility.h>
#include <bakge/core/Bindable.h>
#include <bakge/core/Drawable.h>
#include <bakge/core/Renderer.h>
#include <bakge/core/Engine.h>
#include <bakge/core/EventHandler.h>
#include <bakge/core/Window.h>

/* System modules */
#include <bakge/system/Clock.h>

/* Data structure modules */
#include <bakge/data/File.h>
#include <bakge/data/SingleNode.h>
#include <bakge/data/LinkedList.h>

/* Network modules */
#include <bakge/network/Remote.h>
#include <bakge/network/Packet.h>

/* Include API classes */
#include <bakge/api/Mutex.h>
#include <bakge/api/Thread.h>
#include <bakge/api/Socket.h>

/* Utility headers */
#include <bakge/input/XBoxController.h>

/* Platform headers (may depend on core Bakge classes) */
#ifdef __linux__
#include <bakge/platform/x11_Bakge.h>
#elif defined(_WIN32)
#include <bakge/platform/win32_Bakge.h>
#elif defined(__APPLE__)
#include <bakge/platform/osx_Bakge.h>
#endif /* __linux__ */

/* Math modules */
#include <bakge/math/Math.h>
#include <bakge/math/Vector3.h>
#include <bakge/math/Vector4.h>
#include <bakge/math/Matrix.h>
#include <bakge/math/Quaternion.h>

/* Additional Bakge classes */
#include <bakge/graphics/Shader.h>
#include <bakge/graphics/ShaderProgram.h>
#include <bakge/graphics/Mesh.h>
#include <bakge/graphics/Node.h>
#include <bakge/graphics/Pawn.h>
#include <bakge/graphics/Crowd.h>
#include <bakge/graphics/Shape.h>
#include <bakge/graphics/shapes/Sphere.h>
#include <bakge/graphics/shapes/Cube.h>
#include <bakge/graphics/shapes/Cylinder.h>
#include <bakge/graphics/shapes/Cone.h>
#include <bakge/graphics/shapes/TruncatedPyramid.h>
#include <bakge/graphics/Texture.h>
#include <bakge/graphics/Camera.h>
#include <bakge/renderer/DeferredGeometryRenderer.h>
#include <bakge/renderer/DeferredLightingRenderer.h>
#include <bakge/renderer/FrontRenderer.h>
#include <bakge/engine/ScriptedEngine.h>

#endif /* BAKGE_BAKGE_H */
