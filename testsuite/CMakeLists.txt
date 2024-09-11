set(CMAKE_CXX_STANDARD 23)

enable_testing()

add_executable(testsuite
    main.cxx expect.hxx expect_fallback.hxx)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS")
    target_compile_definitions(testsuite PUBLIC
        IOS_PROCESS_DELAY_WORKAROUND=1)
    set_target_properties(testsuite PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
        XCODE_ATTRIBUTE_ENABLE_BITCODE "NO"
        MACOSX_BUNDLE_BUNDLE_NAME mimicssl-md5-testsuite
        MACOSX_BUNDLE_GUI_IDENTIFIER mimicssl-md5-testsuite
        MACOSX_BUNDLE_BUNDLE_VERSION 1.0
        MACOSX_BUNDLE_SHORT_VERSION_STRING 1.0
        MACOSX_BUNDLE_LONG_VERSION_STRING 1.0)
else()
    target_compile_definitions(testsuite PUBLIC
        IOS_PROCESS_DELAY_WORKAROUND=0)
endif()

target_include_directories(testsuite PRIVATE
    mimicssl-md5 ${CMAKE_SOURCE_DIR}/lighter/include)

target_link_libraries(testsuite mimicssl-md5)

include(GoogleTest)
gtest_discover_tests(testsuite
    DISCOVERY_TIMEOUT 500
    DISCOVERY_MODE PRE_TEST)
