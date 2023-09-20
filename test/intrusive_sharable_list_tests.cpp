#include "data_islist_node.hpp"
#include <arba/itru/intrusive_sharable_list.hpp>
#include <gtest/gtest.h>

//----------------------------------------------------------------

TEST(intrusive_list_iter_tests, constructor_abstract__no_arg__no_exception)
{
    using data_islist_node_isptr = itru::intrusive_shared_ptr<data_islist_node>;

    constexpr std::size_t node_count = 3;
    std::array<data_islist_node_isptr, node_count> nodes =
    {
        itru::make_intrusive_shared_ptr<data_islist_node>(),
        itru::make_intrusive_shared_ptr<data_islist_node>(),
        itru::make_intrusive_shared_ptr<data_islist_node>()
    };
    for (unsigned i = 0; i < node_count; ++i)
    {
        data_islist_node_isptr& node = nodes[i];
        data_islist_node_isptr& next_node = nodes[(i + 1) % node_count];
        data_islist_node_isptr& previous_node = nodes[(i + node_count - 1) % node_count];
        node->text = std::to_string(i);
        node->next() = next_node;
        node->previous() = previous_node.get();
    }

    itru::intrusive_sharable_list_iterator iter(*nodes[0]);
    ASSERT_EQ(iter.ptr(), nodes[0].get());
    ASSERT_EQ(&*iter, nodes[0].get());
    ASSERT_EQ(iter->text, nodes[0]->text);
    ASSERT_EQ(&(iter->text), &(nodes[0]->text));

    itru::intrusive_sharable_list_iterator iter_0(*nodes[0]);
    itru::intrusive_sharable_list_iterator iter_1(*nodes[1]);
    itru::intrusive_sharable_list_iterator iter_2(*nodes[2]);
    ASSERT_EQ(iter, iter_0);
    ASSERT_NE(iter, iter_1);
    ASSERT_NE(iter, iter_2);
    ASSERT_NE(iter_1, iter_2);

    ASSERT_EQ(++iter, iter_1);
    ASSERT_EQ(++iter, iter_2);
    ASSERT_EQ(++iter, iter_0);

    ASSERT_EQ(iter++, iter_0);
    ASSERT_EQ(iter, iter_1);
    ASSERT_EQ(iter++, iter_1);
    ASSERT_EQ(iter, iter_2);
    ASSERT_EQ(iter++, iter_2);
    ASSERT_EQ(iter, iter_0);

    ASSERT_EQ(--iter, iter_2);
    ASSERT_EQ(--iter, iter_1);
    ASSERT_EQ(--iter, iter_0);

    ASSERT_EQ(iter--, iter_0);
    ASSERT_EQ(iter, iter_2);
    ASSERT_EQ(iter--, iter_2);
    ASSERT_EQ(iter, iter_1);
    ASSERT_EQ(iter--, iter_1);
    ASSERT_EQ(iter, iter_0);
}

//----------------------------------------------------------------

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
        auto insert_iter = data_islist.insert(std::next(data_islist.begin()),
                                              itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        ASSERT_EQ(insert_iter, std::next(data_islist.begin()));
        ASSERT_EQ(insert_iter->valid, &value_2);
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

TEST(intrusive_list_tests, pop_front__not_empty_list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        data_islist.pop_front();
        ASSERT_EQ(data_islist.size(), 2);
        ASSERT_FALSE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_TRUE(value_3);
    }
}

TEST(intrusive_list_tests, pop_back__not_empty_list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        data_islist.pop_back();
        ASSERT_EQ(data_islist.size(), 2);
        ASSERT_TRUE(value_1);
        ASSERT_TRUE(value_2);
        ASSERT_FALSE(value_3);
    }
}

TEST(intrusive_list_tests, erase__valid_arg__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::intrusive_sharable_list<data_islist_node> data_islist;
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_1, "1"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_2, "2"));
        data_islist.push_back(itru::make_intrusive_shared_ptr<data_islist_node>(value_3, "3"));
        auto next_iter = data_islist.erase(std::next(data_islist.begin()));
        ASSERT_EQ(next_iter, std::next(data_islist.begin()));
        ASSERT_EQ(next_iter->valid, &value_3);
        ASSERT_EQ(data_islist.size(), 2);
        ASSERT_TRUE(value_1);
        ASSERT_FALSE(value_2);
        ASSERT_TRUE(value_3);
    }
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
        data_islist.clear();
        ASSERT_EQ(data_islist.size(), 0);
        ASSERT_TRUE(data_islist.empty());
    }
    ASSERT_FALSE(svalue);
}
