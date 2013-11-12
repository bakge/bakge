# Bakge CMake file

cmake_minimum_required (VERSION 2.6)

project(BAKGE)

SET(BAKGE_VERSION_STRING "v0.2.0")
SET(BAKGE_VERSION_STRING_FULL "Bakersfield Game Engine v0.2.0")

# Prevent build attempts on any unsupported platform
if(NOT APPLE AND NOT WIN32 AND NOT UNIX)
  message(FATAL_ERROR "Bakge only builds on Windows, OS X or Linux")
endif()

include(${BAKGE_SOURCE_DIR}/cmake/FindSSE.cmake)

if(SSE2_FOUND)
  add_definitions(-DBGE_USE_SIMD)
endif(SSE2_FOUND)

set(BAKGE_SDK_PATH "sdk")
set(BAKGE_LIB_DIR "lib")
set(BAKGE_BIN_DIR "bin")
set(BAKGE_INC_DIR "include")

# Common required packages
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# Build configuration
if(NOT MSVC)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
                                      "MinSizeRel" "RelWithDebInfo")
  endif()
endif()

# Build dynamic/static libraries
option(BUILD_SHARED_LIBS
    "Build Bakge and all dependencies as shared libraries." ON)

# Output all generated libraries and binaries to the SDK dir
set(BAKGE_SDK_DIR ${BAKGE_SOURCE_DIR}/${BAKGE_SDK_PATH})
set(BAKGE_SDK_INC_PATH ${BAKGE_SDK_DIR}/${BAKGE_INC_DIR})
set(BAKGE_SDK_LIB_PATH ${BAKGE_SDK_DIR}/${BAKGE_LIB_DIR}/${CMAKE_BUILD_TYPE})
set(BAKGE_SDK_BIN_PATH ${BAKGE_SDK_DIR}/${BAKGE_BIN_DIR}/${CMAKE_BUILD_TYPE})

# We want ALL libraries (event external ones)to build into the
# SDK's library directory
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BAKGE_SDK_LIB_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BAKGE_SDK_BIN_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BAKGE_SDK_BIN_PATH})

# Bakge options
option(BAKGE_BUILD_TESTS "Build the Bakge test suite" ON)
option(BAKGE_BUILD_EXAMPLES "Build the Bakge examples suite" ON)
option(BAKGE_GDK_BUILD_ENGINE "Build the Bakge GDK engine" ON)

# External libraries included in the source tree
list(APPEND BAKGE_INCLUDE_DIRECTORIES ${BAKGE_SOURCE_DIR}/extern)

# Create a list of all Bakge's build targets
set(BAKGE_TARGET bakge)
set(BAKGE_TARGETS_LIST ${BAKGE_TARGET})

# Library build targets
set(BAKGE_LUA_TARGET lua)
set(BAKGE_GLEW_TARGET glew)
set(BAKGE_PHYSFS_TARGET physfs)

# Append library targets to a list we can use
list(APPEND BAKGE_TARGETS_LIST
  ${BAKGE_PHYSFS_TARGET}
  ${BAKGE_GLEW_TARGET}
  ${BAKGE_LUA_TARGET}
)

########################################
# PLATFORM SPECIFIC LIBRARIES SETUP
########################################

if(UNIX AND NOT APPLE)
  find_package(X11 REQUIRED)
  include_directories(${X11_INCLUDE_DIR})
  include_directories(${X11_LIBRARY_DIR})

  if(BUILD_SHARED_LIBS)
    set(BAKGE_GLFW_TARGET glfw)
  else()
    set(BAKGE_GLFW_TARGET glfw)
  endif()

  set(BAKGE_OPENAL_TARGET openal)

  set(BAKGE_LIBRARIES
    m
    ${BAKGE_GLEW_TARGET}
    ${BAKGE_GLFW_TARGET}
    ${BAKGE_PHYSFS_TARGET}
    lua
    GL
    GLU
    ${X11_X_PRE_LIBS}
    ${X11_LIBRARIES}
    ${X11_X_EXTRA_LIBRARIES}
    ${BAKGE_OPENAL_TARGET}
    Xxf86vm
    Xrandr
    Xi
    pthread
  )
endif()

if(UNIX AND APPLE)
  FIND_LIBRARY(COCOA_LIBRARY Cocoa)
  FIND_LIBRARY(IOKIT_LIBRARY IOKit)
  FIND_LIBRARY(AGL_LIBRARY AGL)
  FIND_LIBRARY(CORE_FOUNDATION_LIBRARY CoreFoundation)
  include_directories(${Cocoa_INCLUDE_DIRS})
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)

  if(BUILD_SHARED_LIBS)
    set(BAKGE_GLFW_TARGET glfw)
  else()
    set(BAKGE_GLFW_TARGET glfw3)
  endif()

  set(BAKGE_LIBRARIES
    m
    ${BAKGE_GLEW_TARGET}
    ${BAKGE_GLFW_TARGET}
    ${BAKGE_PHYSFS_TARGET}
    lua
    ${OPENGL_LIBRARY}
    ${COCOA_LIBRARY}
    ${IOKIT_LIBRARY}
    ${CORE_FOUNDATION_LIBRARY}
    ${AGL_LIBRARY}
  )
endif()

if(WIN32)
  FIND_LIBRARY(OPENGL_LIBRARY OpenGL)

  set(BAKGE_GLFW_TARGET glfw)
  set(BAKGE_OPENAL_TARGET OpenAL32)

  set(BAKGE_LIBRARIES
    ${BAKGE_GLEW_TARGET}
    gdi32.lib
    user32.lib
    winmm.lib
    ${BAKGE_GLFW_TARGET}
    ${BAKGE_PHYSFS_TARGET}
    ${BAKGE_OPENAL_TARGET}
    lua
    glu32.lib
    ${OPENGL_LIBRARY}
    ws2_32.lib
  )
endif(WIN32)

# BAKGE_GLFW_TARGET changes based on platform. Determined above.
list(APPEND BAKGE_TARGETS_LIST ${BAKGE_GLFW_TARGET})

########################################
# BAKGE INCLUDE/LIB PATHS
########################################

# Add include directories
include_directories(
  ${BAKGE_SOURCE_DIR}/include
  ${BAKGE_SOURCE_DIR}/lib/glew-1.9.0/include
  ${BAKGE_SOURCE_DIR}/lib/glfw-3.0/include
  ${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src
  ${BAKGE_SOURCE_DIR}/lib/physfs-2.0.3
  ${BAKGE_SOURCE_DIR}/lib/openal-1.15.1/include
  ${BAKGE_SOURCE_DIR}/extern
)

foreach(directory ${BAKGE_LIBRARY_DIRECTORIES})
  link_directories(${directory})
endforeach(directory)

if(NOT BUILD_SHARED_LIBS)
  link_directories(${BAKGE_SDK_LIB_PATH})
else()
  link_directories(${BAKGE_SDK_BIN_PATH})
endif()

# Configure libraries' options based on our dynamic/static option
if(BUILD_SHARED_LIBS)
  set(PHYSFS_BUILD_STATIC OFF)
  set(PHYSFS_BUILD_SHARED ON)
else()
  add_definitions(-DBAKGE_STATIC)
  set(PHYSFS_BUILD_STATIC ON)
  set(PHYSFS_BUILD_SHARED OFF)
endif()

########################################
# BAKGE CMAKE SUBDIRECTORIES
########################################

# Downloads and builds external library dependencies
add_subdirectory(lib)

# Compile Bakge library
add_subdirectory(src)

# Compiles Bakge unit tests
if(BAKGE_BUILD_TESTS)
  add_subdirectory(test)
endif()

# Compiles example Bakge programs
if(BAKGE_BUILD_EXAMPLES)
  add_subdirectory(example)
endif()

# Build the Bakge GDK Engine
if(BAKGE_GDK_BUILD_ENGINE)
  add_subdirectory(engine)
endif()

########################################
# BAKGE SDK BUILD
########################################

# Create SDK build target
# This copies all headers for Bakge and its libraries into the sdk/
# directory. By default all libraries and run-time binaries are
# output into this directory, so all that's left is to copy headers
add_custom_target(bakge_sdk COMMENT "Building Bakge SDK")
make_directory(${BAKGE_SDK_DIR})
make_directory(${BAKGE_SDK_INC_PATH})

#################################
# Bakge public headers
set(BAKGE_HEADER_MODULES
  api
  audio
  core
  data
  graphics
  input
  interface
  math
  mutex
  platform
  system
  thread
  ui
)
foreach(HEADER_MODULE ${BAKGE_HEADER_MODULES})
  add_custom_command(TARGET bakge_sdk PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy_directory
      ${BAKGE_SOURCE_DIR}/include/bakge/${HEADER_MODULE}
      ${BAKGE_SDK_INC_PATH}/bakge/${HEADER_MODULE}
  )
endforeach()
add_custom_command(TARGET bakge_sdk PRE_BUILD COMMAND ${CMAKE_COMMAND} -E
  copy ${BAKGE_SOURCE_DIR}/include/bakge/Bakge.h ${BAKGE_SDK_INC_PATH}/bakge)

#################################
# PhysicsFS header
set(BAKGE_PHYSFS_HEADERS
  ${BAKGE_SOURCE_DIR}/lib/physfs-2.0.3/physfs.h
)
add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
  copy
    ${BAKGE_SOURCE_DIR}/lib/physfs-2.0.3/physfs.h
    ${BAKGE_SDK_INC_PATH}/physfs.h
)

#################################
# Lua headers
set(BAKGE_LUA_HEADERS
  ${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src/lua.h
  ${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src/lauxlib.h
  ${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src/luaconf.h
  ${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src/lualib.h
)
foreach(LUA_HEADER ${BAKGE_LUA_HEADERS})
  set(PATH_REGEX "${BAKGE_SOURCE_DIR}/lib/lua-5.1.5/src")
  STRING(REGEX REPLACE ${PATH_REGEX}
                "${BAKGE_SDK_INC_PATH}" MOD_HEADER
                ${LUA_HEADER})
  add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy
      ${LUA_HEADER}
      ${MOD_HEADER}
  )
endforeach()

#################################
# stb* headers
set(BAKGE_STB_HEADERS
  stb_truetype.c
  stb_image.c
  stb_vorbis.c
)
foreach(STB_HEADER ${BAKGE_STB_HEADERS})
  add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy
      ${BAKGE_SOURCE_DIR}/extern/stb/${STB_HEADER}
      ${BAKGE_SDK_INC_PATH}/stb/${STB_HEADER}
  )
endforeach()

#################################
# GLEW headers
set(BAKGE_GLEW_HEADERS
  glew.h
  wglew.h
  glxew.h
)
foreach(GLEW_HEADER ${BAKGE_GLEW_HEADERS})
  add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy
      ${BAKGE_SOURCE_DIR}/lib/glew-1.9.0/include/GL/${GLEW_HEADER}
      ${BAKGE_SDK_INC_PATH}/GL/${GLEW_HEADER}
  )
endforeach()

# GLFW headers
set(BAKGE_GLFW_HEADERS
  glfw3.h
  glfw3native.h
)
foreach(GLFW_HEADER ${BAKGE_GLFW_HEADERS})
  add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy
      ${BAKGE_SOURCE_DIR}/lib/glfw-3.0/include/GLFW/${GLFW_HEADER}
      ${BAKGE_SDK_INC_PATH}/GLFW/${GLFW_HEADER}
  )
endforeach()


###########################
# AL headers
set(BAKGE_OPENAL_HEADERS
  al.h
  alc.h
  alext.h
  efx.h
  efx-creative.h
  efx-presets.h
)
foreach(AL_HEADER ${BAKGE_OPENAL_HEADERS})
  add_custom_command(TARGET bakge_sdk POST_BUILD COMMAND ${CMAKE_COMMAND} -E
    copy
      ${BAKGE_SOURCE_DIR}/lib/openal-1.15.1/include/AL/${AL_HEADER}
      ${BAKGE_SDK_INC_PATH}/AL/${AL_HEADER}
  )
endforeach()


######################################################
# BAKGE DOCUMENTATION BUILD (REQUIRES DOXYGEN)
######################################################

find_package(Doxygen)
if(DOXYGEN_FOUND)
  # Generate a configured Doxyfile
  set(BAKGE_DOC_DIR ${BAKGE_SDK_DIR}/doc)
  configure_file(${BAKGE_SOURCE_DIR}/Doxyfile.in ${BAKGE_DOC_DIR}/Doxyfile @ONLY)
  make_directory(${BAKGE_DOC_DIR})
  add_custom_target(bakge_doc ALL ${DOXYGEN_EXECUTABLE} WORKING_DIRECTORY
    ${BAKGE_DOC_DIR} COMMENT "Generating documentation for Bakge" VERBATIM)
endif(DOXYGEN_FOUND)
