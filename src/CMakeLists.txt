# Bakge CMake file

cmake_minimum_required (VERSION 2.6)


########################################
# COMMON HEADERS & MODULES
########################################

set(MODULES
  audio/Environment
  audio/Source
  audio/Stream
  bindings/lua/Common
  data/Decode
  core/Bindable
  core/Drawable
  core/Engine
  core/EventHandler
  core/Utility
  core/Window
  graphics/Camera2D
  graphics/Camera3D
  graphics/Crowd
  graphics/Font
  graphics/Geometry
  graphics/Mesh
  graphics/Node
  graphics/Pawn
  graphics/Shader
  graphics/Shape
  graphics/Texture
  graphics/Viewport
  graphics/geometries/BezierCurve
  graphics/geometries/Grid
  graphics/geometries/Line
  graphics/geometries/LineStrip
  graphics/shapes/Cube
  graphics/shapes/Rectangle
  graphics/shapes/Sphere
  graphics/text/GlyphLibrary
  graphics/text/GlyphMap
  graphics/text/Stamp
  graphics/textures/CompositeTexture
  input/GamePad
  ui/Anchor
  ui/Clickable
  ui/Draggable
  ui/Frame
  ui/Hoverable
  ui/Resizable
  math/Vector3
  math/Vector4
  math/Quaternion
  math/Matrix
  system/Device
)

# Create headers list, add those without a source file
# since they won't get picked up by the following foreach loop
set(HEADERS
  ${BAKGE_SOURCE_DIR}/include/bakge/Bakge
  ${BAKGE_SOURCE_DIR}/include/bakge/math/Math
  ${BAKGE_SOURCE_DIR}/include/bakge/core/Type
  ${BAKGE_SOURCE_DIR}/include/bakge/input/XboxController
  ${BAKGE_SOURCE_DIR}/include/bakge/data/Format
)

foreach(module ${MODULES})
  list(APPEND HEADERS ${BAKGE_SOURCE_DIR}/include/bakge/${module})
endforeach(module)


########################################
# PLATFORM MODULES & HEADERS
########################################

if(UNIX AND NOT APPLE)
  set(PLATFORM_PREFIX "x11")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -march=native")
endif()

if(UNIX AND APPLE)
  set(PLATFORM_PREFIX "osx")
  list(APPEND CMAKE_CXX_FLAGS -ObjC++)
endif()

if(WIN32)
  set(PLATFORM_PREFIX "win32")
endif()

set(PLATFORM_MODULES
  clock/${PLATFORM_PREFIX}_Clock
  thread/${PLATFORM_PREFIX}_Thread
  mutex/${PLATFORM_PREFIX}_Mutex
  utility/${PLATFORM_PREFIX}_Utility
)

# Platform modules might share a common header.
# Manually set them here
set(PLATFORM_HEADERS
  ${BAKGE_SOURCE_DIR}/include/bakge/system/Clock
  ${BAKGE_SOURCE_DIR}/include/bakge/core/Utility
  ${BAKGE_SOURCE_DIR}/include/bakge/mutex/${PLATFORM_PREFIX}_Mutex
  ${BAKGE_SOURCE_DIR}/include/bakge/thread/${PLATFORM_PREFIX}_Thread
)

# Extern libraries in the source tree
set(EXTERN_SOURCES
  ${BAKGE_SOURCE_DIR}/extern/stb/stb_image.c
  ${BAKGE_SOURCE_DIR}/extern/stb/stb_truetype.c
  ${BAKGE_SOURCE_DIR}/extern/stb/stb_vorbis.c
)

set(INTERNAL_SOURCES
  internal/Debug
  internal/Utility
)

set(INTERNAL_HEADERS
  ${BAKGE_SOURCE_DIR}/include/bakge/internal/Debug
  ${BAKGE_SOURCE_DIR}/include/bakge/internal/Utility
  ${BAKGE_SOURCE_DIR}/include/bakge/internal/Format
  ${BAKGE_SOURCE_DIR}/include/bakge/internal/Graphics
)

if(BUILD_SHARED_LIBS)
  # It's a matter of taste
  remove_definitions(-Dbakge_EXPORTS)
  add_definitions(-DBAKGE_EXPORTS)
endif()

add_definitions(-DSTB_TRUETYPE_IMPLEMENTATION)
add_definitions(-DHAVE_SNPRINTF)
add_definitions(-DPREFER_PORTABLE_SNPRINTF)
add_library(bakge ${MODULES} ${PLATFORM_MODULES} ${HEADERS} ${EXTERN_SOURCES}
                                    ${PLATFORM_HEADERS} ${INTERNAL_SOURCES})
if(BUILD_SHARED_LIBS)
  target_link_libraries(bakge ${BAKGE_LIBRARIES})
  target_link_libraries(bakge ${BAKGE_BULLET_TARGETS})
endif()
