set(CMAKE_C_STANDARD 23)

add_executable(mimicssl-md5-cli main.c)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
    set_target_properties(mimicssl-md5-cli PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        MACOSX_BUNDLE_BUNDLE_NAME mimicssl-md5-cli
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0)
endif()

target_include_directories(mimicssl-md5-cli PRIVATE mimicssl-md5)

target_link_libraries(mimicssl-md5-cli mimicssl-md5)
