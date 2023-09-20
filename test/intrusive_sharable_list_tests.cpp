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

TEST(intrusive_list_tests, push_back__valid_arg__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        ASSERT_FALSE(data_islist.empty());
        ASSERT_EQ(data_islist.size(), 1);
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        ASSERT_EQ(data_islist.size(), 2);
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.size(), 3);
        auto iter = data_islist.begin();
        ASSERT_EQ(iter->valid, &value_1);
        ++iter;
        ASSERT_EQ(iter->valid, &value_2);
        ++iter;
        ASSERT_EQ(iter->valid, &value_3);
        ASSERT_TRUE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_TRUE(value_3);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value_2);
    ASSERT_FALSE(value_3);
}

TEST(intrusive_list_tests, push_front__valid_arg__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_front(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        ASSERT_FALSE(data_islist.empty());
        ASSERT_EQ(data_islist.size(), 1);
        data_islist.push_front(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        ASSERT_EQ(data_islist.size(), 2);
        data_islist.push_front(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.size(), 3);
        auto iter = data_islist.begin();
        ASSERT_EQ(iter->valid, &value_3);
        ++iter;
        ASSERT_EQ(iter->valid, &value_2);
        ++iter;
        ASSERT_EQ(iter->valid, &value_1);
        ASSERT_TRUE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_TRUE(value_3);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value_2);
    ASSERT_FALSE(value_3);
}

TEST(intrusive_list_tests, insert__valid_args__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        ASSERT_EQ(data_islist.size(), 1);
        ASSERT_FALSE(data_islist.empty());
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.size(), 2);
        data_islist.insert(std::next(data_islist.begin()),
                           itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        ASSERT_EQ(data_islist.size(), 3);
        auto iter = data_islist.begin();
        ASSERT_EQ(iter->valid, &value_1);
        ++iter;
        ASSERT_EQ(iter->valid, &value_2);
        ++iter;
        ASSERT_EQ(iter->valid, &value_3);
        ASSERT_TRUE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_TRUE(value_3);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value_2);
    ASSERT_FALSE(value_3);
}

TEST(intrusive_list_tests, clear__not_empty_list__no_exception)
{
    bool svalue = false;
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist(data_islist_node(svalue, "sentinel"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        ASSERT_TRUE(svalue);
        ASSERT_TRUE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_TRUE(value_3);
        ASSERT_EQ(data_islist.size(), 3);
        ASSERT_FALSE(data_islist.empty());
        data_islist.clear();
        ASSERT_TRUE(svalue);
        ASSERT_FALSE(value_1);
        ASSERT_FALSE(value_2);
        ASSERT_FALSE(value_3);
        ASSERT_EQ(data_islist.size(), 0);
        ASSERT_TRUE(data_islist.empty());
    }
    ASSERT_FALSE(svalue);
}
