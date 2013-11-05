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

/*!
 * @file Bakge.h
 * @brief Includes all Bakge classes, namespaces, type definitions and
 * public API.
 */

#ifndef BAKGE_BAKGE_H
#define BAKGE_BAKGE_H

#ifndef _DEBUG
#ifndef NDEBUG
#define _DEBUG
#endif // NDEBUG
#endif // _DEBUG

/* Include standard library dependencies */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef __linux__
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#elif defined(_WIN32)
// Disable some pesky MSVC warnings
#pragma warning(disable : 4193)
#pragma warning(disable : 4005)
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#elif defined(__APPLE__)
#import <cocoa/Cocoa.h>
#include <unistd.h>
#endif // __linux__

/* GCC & Clang attributes */
#if defined __GNUC__ || defined __clang__ || defined __MINGW__
/*! @brief Send compiler warning if function return result is not used.
 */
#define BGE_WUNUSED __attribute__((warn_unused_result))
/*! @brief Mark a variable as possibly unused in its function.
 */
#define BGE_UNUSED __attribute__((unused))
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
/*! @brief Align memory to provided boundaries.
 *
 * See your compiler/platform manuals or documentation for more information on
 * memory alignment.
 */
#define BGE_ALIGN(X) __attribute__((aligned(X)))
#endif

/*! @brief Declaration for Bakge global API.
 */
#define BGE_FUNC extern BGE_API

/*! @brief Factory functions are static methods which allocate heap memory.
 *
 * If available, compilers will raise warnings when the result from a factory
 * function is unused (often leading to memory leaks).
 */
#define BGE_FACTORY static BGE_WUNUSED

/*! @brief No-copy const reference qualifier. Commonly used for passing math
 * classes.
 */
#define BGE_NCP const&

/*! @brief Inline hint macro.
 */
#define BGE_INL inline

/*! @brief Current release major version number.
 */
#define BGE_VER_MAJ 0

/*! @brief Current release minor version number.
 */
#define BGE_VER_MIN 1

/*! @brief Current release revision version number.
 */
#define BGE_VER_REV 0

/* Include external library headers */
#include <GL/glew.h>
#include <GLFW/glfw3.h>
extern "C"
{
#include <lua.h>
#include <luaconf.h>
#include <lualib.h>
#include <lauxlib.h>
}
#include <physfs.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>

/*! @brief All Bakge types, classes and API are declared and defined in this
 * namespace.
 */
namespace bakge
{
}

/* Include other external libraries */
#define STBI_HEADER_FILE_ONLY
#include <stb/stb_image.c>
#undef STBI_HEADER_FILE_ONLY
#ifdef STB_TRUETYPE_IMPLEMENTATION
#undef STB_TRUETYPE_IMPLEMENTATION
#endif /* STB_TRUETYPE_IMPLEMENTATION */
#include <stb/stb_truetype.c>
#define STB_TRUETYPE_IMPLEMENTATION
#ifndef STB_VORBIS_HEADER_ONLY
#define STB_VORBIS_HEADER_ONLY
#endif // STB_VORBIS_HEADER_ONLY
#include <stb/stb_vorbis.c>
#undef STB_VORBIS_HEADER_ONLY

/* Include core Bakge classes and headers */
#include <bakge/core/Type.h>
#include <bakge/core/Input.h>
#include <bakge/core/Utility.h>
#include <bakge/core/Bindable.h>
#include <bakge/core/Drawable.h>
#include <bakge/core/Engine.h>
#include <bakge/core/EventHandler.h>
#include <bakge/core/Window.h>

/* System modules */
#include <bakge/system/Clock.h>
#include <bakge/system/Device.h>

/* Data/filesystem module */
#include <bakge/data/Format.h>
#include <bakge/data/Decode.h>

/* Utility headers */
#include <bakge/input/GamePad.h>
#include <bakge/input/XboxController.h>

#ifdef __linux__
#include <bakge/mutex/x11_Mutex.h>
#include <bakge/thread/x11_Thread.h>
#elif defined(_WIN32)
#include <bakge/mutex/win32_Mutex.h>
#include <bakge/thread/win32_Thread.h>
#elif defined(__APPLE__)
#include <bakge/mutex/osx_Mutex.h>
#include <bakge/thread/osx_Thread.h>
#endif // __linux__

/* Math modules */
#include <bakge/math/Math.h>
#include <bakge/math/Vector3.h>
#include <bakge/math/Vector4.h>
#include <bakge/math/Matrix.h>
#include <bakge/math/Quaternion.h>

/* Additional Bakge classes */
#include <bakge/graphics/Viewport.h>
#include <bakge/graphics/Shape.h>
#include <bakge/graphics/Shader.h>
#include <bakge/graphics/Mesh.h>
#include <bakge/graphics/Node.h>
#include <bakge/graphics/Pawn.h>
#include <bakge/graphics/Crowd.h>
#include <bakge/graphics/Geometry.h>
#include <bakge/graphics/shapes/Cube.h>
#include <bakge/graphics/shapes/Rectangle.h>
#include <bakge/graphics/shapes/Sphere.h>
#include <bakge/graphics/geometries/Line.h>
#include <bakge/graphics/geometries/LineStrip.h>
#include <bakge/graphics/geometries/Grid.h>
#include <bakge/graphics/geometries/BezierCurve.h>
#include <bakge/graphics/Texture.h>
#include <bakge/graphics/textures/AnimationTexture.h>
#include <bakge/graphics/textures/CompositeTexture.h>
#include <bakge/graphics/Font.h>
#include <bakge/graphics/text/Stamp.h>
#include <bakge/graphics/text/GlyphMap.h>
#include <bakge/graphics/text/GlyphLibrary.h>
#include <bakge/graphics/Camera2D.h>
#include <bakge/graphics/Camera3D.h>
#include <bakge/ui/Anchor.h>
#include <bakge/ui/Frame.h>
#include <bakge/ui/Hoverable.h>
#include <bakge/ui/Clickable.h>
#include <bakge/ui/Draggable.h>
#include <bakge/ui/Resizable.h>

/* Audio classes */
#include <bakge/audio/Environment.h>
#include <bakge/audio/Source.h>
#include <bakge/audio/Stream.h>

#include <bakge/bindings/lua/Common.h>

#endif /* BAKGE_BAKGE_H */
