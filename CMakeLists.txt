cmake_minimum_required(VERSION 3.27)

project("mimicssl-md5" VERSION 1.0.0)

cmake_policy(SET CMP0142 NEW)

include(CTest)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_subdirectory(libmimicssl-md5)
add_subdirectory(mimicssl-md5-cli)
add_subdirectory(testsuite)
