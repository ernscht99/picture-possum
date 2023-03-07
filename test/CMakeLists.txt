set(TEST_SOURCES
        # add your source files here
        FindFilesTest.cpp
        Tester.cpp TestExif.cpp)
#enable_testing()
find_package(GTest REQUIRED)

add_executable(tester ${TEST_SOURCES})
target_include_directories(tester PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tester PUBLIC
        possum_core
        GTest::GTest)
