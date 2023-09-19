
find_or_fetch_GTest()

add_cpp_library_basic_tests(GTest::gtest_main
    HEADER_ONLY ${PROJECT_NAME}
    SOURCES
        project_version_tests.cpp
        intrusive_ref_counter_tests.cpp
        intrusive_ref_counters_tests.cpp
        isptr_with_custom_counter_tests.cpp
        isptr_with_core_counter_tests.cpp
        make_isptr_tests.cpp
        iwptr_with_core_counter_tests.cpp
        intrusive_sharable_list_tests.cpp
)
