#include <arba/itru/version.hpp>
#include <gtest/gtest.h>
#include <format>

TEST(project_version_tests, test_version_macros)
{
    const unsigned major = 0;
    const unsigned minor = 2;
    const unsigned patch = 0;
    ASSERT_EQ(ARBA_ITRU_VERSION_MAJOR, major);
    ASSERT_EQ(ARBA_ITRU_VERSION_MINOR, minor);
    ASSERT_EQ(ARBA_ITRU_VERSION_PATCH, patch);
    ASSERT_EQ(ARBA_ITRU_VERSION, std::format("{}.{}.{}", major, minor, patch));
}
