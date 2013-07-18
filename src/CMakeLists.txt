# Bakge CMake file

cmake_minimum_required (VERSION 2.6)


########################################
# COMMON HEADERS & MODULES
########################################

set(MODULES
  api/Mutex
  api/Socket
  api/Thread
  data/File
  core/Bindable
  core/Drawable
  core/Engine
  core/EventHandler
  core/Renderer
  core/Utility
  core/Window
  engine/ScriptedEngine
  graphics/Camera
  graphics/Crowd
  graphics/Mesh
  graphics/Node
  graphics/Pawn
  graphics/Shader
  graphics/ShaderProgram
  graphics/Shape
  graphics/Texture
  graphics/shapes/Sphere
  graphics/shapes/Cone
  graphics/shapes/Cube
  graphics/shapes/Cylinder
  graphics/shapes/Pyramid
  math/Vector3
  math/Vector4
  math/Quaternion
  math/Matrix
  network/Packet
  network/Remote
  renderer/DeferredGeometryRenderer
  renderer/DeferredLightingRenderer
  renderer/FrontRenderer
)

# Create headers list, add those without a source file
# since they won't get picked up by the following foreach loop
set(HEADERS
  ${BAKGE_SOURCE_DIR}/include/bakge/Bakge
  ${BAKGE_SOURCE_DIR}/include/bakge/math/Math
  ${BAKGE_SOURCE_DIR}/include/bakge/core/Type
  ${BAKGE_SOURCE_DIR}/include/bakge/data/LinkedList
  ${BAKGE_SOURCE_DIR}/include/bakge/data/SingleNode
)

foreach(module ${MODULES})
  list(APPEND HEADERS ${BAKGE_SOURCE_DIR}/include/bakge/${module})
endforeach(module)


########################################
# PLATFORM MODULES & HEADERS
########################################

if(UNIX AND NOT APPLE)
  set(PLATFORM_PREFIX "x11")
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
  socket/${PLATFORM_PREFIX}_Socket
  thread/${PLATFORM_PREFIX}_Thread
  mutex/${PLATFORM_PREFIX}_Mutex
  utility/${PLATFORM_PREFIX}_Utility
)

# Platform modules might share a common header.
# Manually set them here
set(PLATFORM_HEADERS
  ${BAKGE_SOURCE_DIR}/include/bakge/platform/${PLATFORM_PREFIX}_Bakge
  ${BAKGE_SOURCE_DIR}/include/bakge/system/Clock
  ${BAKGE_SOURCE_DIR}/include/bakge/core/Utility
  ${BAKGE_SOURCE_DIR}/include/bakge/mutex/${PLATFORM_PREFIX}_Mutex
  ${BAKGE_SOURCE_DIR}/include/bakge/thread/${PLATFORM_PREFIX}_Thread
  ${BAKGE_SOURCE_DIR}/include/bakge/socket/${PLATFORM_PREFIX}_Socket
)

# Extern libraries in the source tree
set(EXTERN_SOURCES
  ${BAKGE_SOURCE_DIR}/extern/stb/stb_image.c
  ${BAKGE_SOURCE_DIR}/extern/stb/stb_truetype.h
)

if(BAKGE_BUILD_DYNAMIC)
  # It's a matter of taste
  remove_definitions(-Dbakge_EXPORTS)
  add_definitions(-DBAKGE_EXPORTS)
endif()

add_library(bakge ${MODULES} ${PLATFORM_MODULES} ${HEADERS}
                                        ${PLATFORM_HEADERS} ${EXTERN_SOURCES})
if(BAKGE_BUILD_DYNAMIC)
  target_link_libraries(bakge ${BAKGE_LIBRARIES})
endif()
