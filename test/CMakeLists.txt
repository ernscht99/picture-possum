set(TEST_SOURCES
        # add your source files here
        Tester.cpp)

add_executable(tester ${TEST_SOURCES})
target_link_libraries(tester PUBLIC
        pljit_core
        GTest::GTest)
