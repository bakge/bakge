# Bakge Official engine CMake file

cmake_minimum_required (VERSION 2.6)

set(BAKGE_GDK_DIR ${CMAKE_SOURCE_DIR}/gdk)
set(BAKGE_GDK_ENGINE_DIR ${CMAKE_SOURCE_DIR}/engine)

include_directories(${BAKGE_GDK_ENGINE_DIR}/include)

set(BAKGE_GDK_ENGINE_HEADERS
  ${BAKGE_SOURCE_DIR}/engine/include/bakge/gdk/engine/Common
  ${BAKGE_SOURCE_DIR}/engine/include/bakge/gdk/engine/Application
  ${BAKGE_SOURCE_DIR}/engine/include/bakge/gdk/engine/Config
  ${BAKGE_SOURCE_DIR}/engine/include/bakge/gdk/engine/GUI
  ${BAKGE_SOURCE_DIR}/engine/include/bakge/gdk/engine/ui/Element
)

set(BAKGE_GDK_ENGINE_SOURCES
  ${BAKGE_SOURCE_DIR}/engine/src/gdk/engine/main
  ${BAKGE_SOURCE_DIR}/engine/src/gdk/engine/Application
  ${BAKGE_SOURCE_DIR}/engine/src/gdk/engine/GUI
  ${BAKGE_SOURCE_DIR}/engine/src/gdk/engine/ui/Element
)

# So Bakge engine executable can find shared libs in its directory
# Without this dynamic linking won't work on Linux
link_directories(${BAKGE_GDK_DIR}/${CMAKE_BUILD_TYPE})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BAKGE_GDK_DIR}/${CMAKE_BUILD_TYPE})
add_executable(BakgeEngine ${BAKGE_GDK_ENGINE_HEADERS} ${BAKGE_GDK_ENGINE_SOURCES})
target_link_libraries(BakgeEngine bakge ${BAKGE_LIBRARIES})

if(UNIX AND APPLE)
  list(APPEND CMAKE_CXX_FLAGS "-ObjC++")
endif()

# Copy libraries over to GDK directory
add_custom_target(GDKEngineCopyLibs)

if(BUILD_SHARED_LIBS)
  foreach(COPYLIB ${BAKGE_TARGETS_LIST})
    add_custom_command(TARGET GDKEngineCopyLibs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${COPYLIB}>
        ${BAKGE_GDK_DIR}/${CMAKE_BUILD_TYPE})

    # If we're on Linux there may be other generated files
    if(UNIX AND NOT APPLE)
      add_custom_command(TARGET GDKEngineCopyLibs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_LINKER_FILE:${COPYLIB}>
        ${BAKGE_GDK_DIR}/${CMAKE_BUILD_TYPE})
      add_custom_command(TARGET GDKEngineCopyLibs POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_SONAME_FILE:${COPYLIB}>
        ${BAKGE_GDK_DIR}/${CMAKE_BUILD_TYPE})
    endif()
  endforeach()
endif()

add_dependencies(GDKEngineCopyLibs ${BAKGE_TARGETS_LIST})
add_dependencies(BakgeEngine GDKEngineCopyLibs)

