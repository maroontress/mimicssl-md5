set(CMAKE_C_STANDARD 23)

add_executable(mimicssl-md5-cli main.c)

target_include_directories(mimicssl-md5-cli PRIVATE mimicssl-md5)

target_link_libraries(mimicssl-md5-cli mimicssl-md5)
