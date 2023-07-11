set(CMAKE_CXX_STANDARD 23)

enable_testing()

add_executable(testsuite
    main.cxx expect.hxx expect_fallback.hxx)

target_include_directories(testsuite PRIVATE
    mimicssl-md5 ${CMAKE_SOURCE_DIR}/lighter/include)

target_link_libraries(testsuite mimicssl-md5)

include(GoogleTest)
gtest_discover_tests(testsuite)
