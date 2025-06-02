include(cmtk/CppLibraryTests)
include(GoogleTest)

find_package(GTest 1.14 CONFIG REQUIRED)

add_cpp_library_basic_tests(${PROJECT_NAME} GTest::gtest_main
    SOURCES
        project_version_tests.cpp
        intrusive_ref_counter_tests.cpp
        intrusive_ref_counters_tests.cpp
        siptr_with_custom_counter_tests.cpp
        siptr_with_core_counter_tests.cpp
        make_siptr_tests.cpp
        wiptr_with_core_counter_tests.cpp
        sharable_intrusive_list_tests.cpp
)
