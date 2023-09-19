#include "data_islist_node.hpp"
#include <arba/itru/intrusive_sharable_list.hpp>
#include <gtest/gtest.h>

TEST(intrusive_list_tests, constructor_abstract__no_arg__no_exception)
{
    itru::intrusive_sharable_list<abstract_data_islist_node, sentinel_data_islist_node> data_islist;
    ASSERT_EQ(data_islist.size(), 0);
    ASSERT_TRUE(data_islist.empty());
}

TEST(intrusive_list_tests, constructor__no_arg__no_exception)
{
    itru::intrusive_sharable_list<data_islist_node> data_islist;
    ASSERT_EQ(data_islist.size(), 0);
    ASSERT_TRUE(data_islist.empty());
    ASSERT_EQ(data_islist.begin(), data_islist.end());
    ASSERT_EQ(data_islist.cbegin(), data_islist.cend());
}

TEST(intrusive_list_tests, constructor__sentinel_arg__no_exception)
{
    bool svalue = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist(data_islist_node(svalue, "sentinel"));
        ASSERT_TRUE(svalue);
        ASSERT_EQ(data_islist.size(), 0);
        ASSERT_TRUE(data_islist.empty());
    }
    ASSERT_FALSE(svalue);
}
