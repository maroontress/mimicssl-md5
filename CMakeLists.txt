cmake_minimum_required(VERSION 3.24)

project("mimicssl-md5" VERSION 1.0.0)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(libmimicssl-md5)
add_subdirectory(mimicssl-md5-cli)

option(BUILD_TESTSUITE "Build testsuite" ON)
if (${BUILD_TESTSUITE})
  include(CTest)
  add_subdirectory(testsuite)
endif()
