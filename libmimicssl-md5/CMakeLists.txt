set(CMAKE_C_STANDARD 23)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"
        OR "${CMAKE_C_COMPILER_ID}" STREQUAL "AppleClang"
        OR "${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic)
    set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
    set(CMAKE_C_FLAGS_RELEASE "-O3")
elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

include(GenerateExportHeader)

add_library(mimicssl-md5 STATIC)
generate_export_header(mimicssl-md5
    BASE_NAME MD5
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/md5_export.h)
target_sources(mimicssl-md5 PRIVATE
    src/md5.c)
target_include_directories(mimicssl-md5 PUBLIC
    include
    ${PROJECT_BINARY_DIR})

add_library(mimicssl-md5-shared SHARED)
if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
    set_target_properties(mimicssl-md5-shared PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        MACOSX_BUNDLE_BUNDLE_NAME mimicssl-md5
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0)
endif()
target_sources(mimicssl-md5-shared PRIVATE
    src/md5.c)
target_include_directories(mimicssl-md5-shared PUBLIC
    include
    ${PROJECT_BINARY_DIR})
set_target_properties(mimicssl-md5-shared
    PROPERTIES OUTPUT_NAME mimicssl-md5)

include(GNUInstallDirs)
install(TARGETS mimicssl-md5 DESTINATION lib)
install(TARGETS mimicssl-md5-shared DESTINATION lib)
install(FILES
    include/md5.h
    ${PROJECT_BINARY_DIR}/md5_export.h
    DESTINATION include/mimicssl)
