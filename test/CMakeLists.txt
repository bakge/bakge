# Bakge unit tests CMake file

cmake_minimum_required(VERSION 2.6)

if(BAKGE_BUILD_SDK)
  set(TESTS_SUBDIR "test")
endif()

set(TESTS
  audio
  bezier
  crowd
  animationtexture
  compositetexture
  device
  font
  grid
  rectangle
  thread
)

if(NOT APPLE)
  foreach(test ${TESTS})
    add_executable(${test} ${test}.cpp)
    target_link_libraries(${test} bakge ${BAKGE_LIBRARIES})
  endforeach(test)
else()
  list(APPEND CMAKE_CXX_FLAGS -ObjC++)
  foreach(test ${TESTS})
    add_executable(${test} MACOSX_BUNDLE ${test})
    target_link_libraries(${test} bakge ${BAKGE_LIBRARIES})
    set_target_properties(${test} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME ${test})
    set_target_properties(${test} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING ${BAKGE_VERSION_STRING}
                                             MACOSX_BUNDLE_LONG_VERSION_STRING ${BAKGE_VERSION_STRING_FULL})
  endforeach(test)
endif()
