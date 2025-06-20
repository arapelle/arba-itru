#include "data_silist_node.hpp"
#include <arba/itru/sharable_intrusive_list.hpp>

#include <gtest/gtest.h>

//----------------------------------------------------------------

TEST(intrusive_list_iter_tests, constructor_abstract__no_arg__no_exception)
{
    using data_islist_node_siptr = itru::shared_intrusive_ptr<data_silist_node>;

    constexpr std::size_t node_count = 3;
    std::array<data_islist_node_siptr, node_count> nodes = { itru::make_shared_intrusive_ptr<data_silist_node>(),
                                                             itru::make_shared_intrusive_ptr<data_silist_node>(),
                                                             itru::make_shared_intrusive_ptr<data_silist_node>() };
    for (unsigned i = 0; i < node_count; ++i)
    {
        data_islist_node_siptr& node = nodes[i];
        data_islist_node_siptr& next_node = nodes[(i + 1) % node_count];
        data_islist_node_siptr& previous_node = nodes[(i + node_count - 1) % node_count];
        node->text = std::to_string(i);
        node->next() = next_node;
        node->previous() = previous_node.get();
    }

    itru::sharable_intrusive_list_iterator iter(*nodes[0]);
    ASSERT_EQ(iter.ptr(), nodes[0].get());
    ASSERT_EQ(&*iter, nodes[0].get());
    ASSERT_EQ(iter->text, nodes[0]->text);
    ASSERT_EQ(&(iter->text), &(nodes[0]->text));

    itru::sharable_intrusive_list_iterator iter_0(*nodes[0]);
    itru::sharable_intrusive_list_iterator iter_1(*nodes[1]);
    itru::sharable_intrusive_list_iterator iter_2(*nodes[2]);
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
    itru::sharable_intrusive_list<abstract_data_silist_node, sentinel_data_silist_node> data_islist;
    ASSERT_EQ(data_islist.size(), 0);
    ASSERT_TRUE(data_islist.empty());
}

TEST(intrusive_list_tests, constructor__no_arg__no_exception)
{
    itru::sharable_intrusive_list<data_silist_node> data_islist;
    ASSERT_EQ(data_islist.size(), 0);
    ASSERT_TRUE(data_islist.empty());
    ASSERT_EQ(data_islist.begin(), data_islist.end());
    ASSERT_EQ(data_islist.cbegin(), data_islist.cend());
}

TEST(intrusive_list_tests, constructor__sentinel_arg__no_exception)
{
    bool svalue = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist(data_silist_node(svalue, "sentinel"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        ASSERT_FALSE(data_islist.empty());
        ASSERT_EQ(data_islist.size(), 1);
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        ASSERT_EQ(data_islist.size(), 2);
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_front(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        ASSERT_FALSE(data_islist.empty());
        ASSERT_EQ(data_islist.size(), 1);
        data_islist.push_front(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        ASSERT_EQ(data_islist.size(), 2);
        data_islist.push_front(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        ASSERT_TRUE(data_islist.empty());
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        ASSERT_EQ(data_islist.size(), 1);
        ASSERT_FALSE(data_islist.empty());
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.size(), 2);
        auto insert_iter = data_islist.insert(std::next(data_islist.begin()),
                                              itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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
        itru::sharable_intrusive_list<data_silist_node> data_islist(data_silist_node(svalue, "sentinel"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
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

TEST(intrusive_list_tests, swap__not_empty_list_arg__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        itru::sharable_intrusive_list<data_silist_node> other_data_islist;
        other_data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        data_islist.swap(other_data_islist);
        ASSERT_EQ(data_islist.size(), 1);
        ASSERT_EQ(data_islist.begin()->valid, &value_3);
        ASSERT_EQ(other_data_islist.size(), 2);
        ASSERT_EQ(other_data_islist.begin()->valid, &value_1);
        ASSERT_EQ((--other_data_islist.end())->valid, &value_2);
    }
}

TEST(intrusive_list_tests, std_swap__not_empty_list_arg__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        itru::sharable_intrusive_list<data_silist_node> other_data_islist;
        other_data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        std::swap(data_islist, other_data_islist);
        ASSERT_EQ(data_islist.size(), 1);
        ASSERT_EQ(data_islist.begin()->valid, &value_3);
        ASSERT_EQ(other_data_islist.size(), 2);
        ASSERT_EQ(other_data_islist.begin()->valid, &value_1);
        ASSERT_EQ((--other_data_islist.end())->valid, &value_2);
    }
}

TEST(intrusive_list_tests, front__not_empty_list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.front().valid, &value_1);
        const itru::sharable_intrusive_list<data_silist_node>& data_islist_cref = data_islist;
        ASSERT_EQ(data_islist_cref.front().valid, &value_1);
    }
}

TEST(intrusive_list_tests, back__not_empty_list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_2, "2"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        ASSERT_EQ(data_islist.back().valid, &value_3);
        const itru::sharable_intrusive_list<data_silist_node>& data_islist_cref = data_islist;
        ASSERT_EQ(data_islist_cref.back().valid, &value_3);
    }
}

TEST(intrusive_list_tests, emplace_front__valid_args__no_exception)
{
    bool value = false;
    bool value_1 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value, "."));
        ASSERT_FALSE(value_1);
        data_islist.emplace_front(value_1, "1");
        ASSERT_TRUE(value_1);
        ASSERT_EQ(data_islist.front().valid, &value_1);
        ASSERT_EQ(data_islist.front().text, "1");
        ASSERT_EQ(data_islist.size(), 2);
        ASSERT_EQ(data_islist.back().valid, &value);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value);
}

TEST(intrusive_list_tests, emplace_back__valid_args__no_exception)
{
    bool value = false;
    bool value_1 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value, "."));
        ASSERT_FALSE(value_1);
        data_islist.emplace_back(value_1, "1");
        ASSERT_TRUE(value_1);
        ASSERT_EQ(data_islist.back().valid, &value_1);
        ASSERT_EQ(data_islist.back().text, "1");
        ASSERT_EQ(data_islist.size(), 2);
        ASSERT_EQ(data_islist.front().valid, &value);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value);
}

TEST(intrusive_list_tests, emplace__valid_args__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_1, "1"));
        data_islist.push_back(itru::make_shared_intrusive_ptr<data_silist_node>(value_3, "3"));
        ASSERT_FALSE(value_2);
        auto iter = data_islist.emplace(std::next(data_islist.begin()), value_2, "2");
        ASSERT_TRUE(value_2);
        ASSERT_EQ(iter, std::next(data_islist.begin()));
        ASSERT_EQ(iter->valid, &value_2);
        ASSERT_EQ(iter->text, "2");
        ASSERT_EQ(data_islist.size(), 3);
        ASSERT_EQ(data_islist.front().valid, &value_1);
        ASSERT_EQ(data_islist.back().valid, &value_3);
    }
    ASSERT_FALSE(value_1);
    ASSERT_FALSE(value_2);
    ASSERT_FALSE(value_3);
}

TEST(intrusive_list_tests, splice__list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "2");
        data_islist.emplace_back(value_3, "3");
        itru::sharable_intrusive_list<data_silist_node> other_data_islist;
        other_data_islist.emplace_back(value_4, "4");
        other_data_islist.emplace_back(value_5, "5");
        other_data_islist.emplace_back(value_6, "6");
        data_islist.splice(data_islist.begin(), other_data_islist);
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
            value_pointers.push_back(item.valid);
        std::vector<const bool*> expected_pointers{ &value_4, &value_5, &value_6, &value_1, &value_2, &value_3 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_EQ(data_islist.size(), 6);
        ASSERT_TRUE(other_data_islist.empty());
    }
}

TEST(intrusive_list_tests, splice__rvalue_list__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "2");
        data_islist.emplace_back(value_3, "3");
        auto gen_list = [&]()
        {
            itru::sharable_intrusive_list<data_silist_node> other_data_islist;
            other_data_islist.emplace_back(value_4, "4");
            other_data_islist.emplace_back(value_5, "5");
            other_data_islist.emplace_back(value_6, "6");
            return other_data_islist;
        };
        data_islist.splice(data_islist.begin(), gen_list());
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
            value_pointers.push_back(item.valid);
        std::vector<const bool*> expected_pointers{ &value_4, &value_5, &value_6, &value_1, &value_2, &value_3 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_EQ(data_islist.size(), 6);
    }
}

TEST(intrusive_list_tests, splice__sublist__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "2");
        data_islist.emplace_back(value_3, "3");
        itru::sharable_intrusive_list<data_silist_node> other_data_islist;
        other_data_islist.emplace_back(value_4, "4");
        other_data_islist.emplace_back(value_5, "5");
        other_data_islist.emplace_back(value_6, "6");
        data_islist.splice(data_islist.begin(), other_data_islist, std::next(other_data_islist.begin()),
                           other_data_islist.end());
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
            value_pointers.push_back(item.valid);
        std::vector<const bool*> expected_pointers{ &value_5, &value_6, &value_1, &value_2, &value_3 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_EQ(data_islist.size(), 5);
        ASSERT_EQ(other_data_islist.front().valid, &value_4);
        ASSERT_EQ(other_data_islist.size(), 1);
    }
}

TEST(intrusive_list_tests, splice__value_iter__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "2");
        data_islist.emplace_back(value_3, "3");
        itru::sharable_intrusive_list<data_silist_node> other_data_islist;
        other_data_islist.emplace_back(value_4, "4");
        other_data_islist.emplace_back(value_5, "5");
        other_data_islist.emplace_back(value_6, "6");
        data_islist.splice(data_islist.begin(), other_data_islist, std::next(other_data_islist.begin()));
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
            value_pointers.push_back(item.valid);
        std::vector<const bool*> expected_pointers{ &value_5, &value_1, &value_2, &value_3 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_EQ(data_islist.size(), 4);
        ASSERT_EQ(other_data_islist.front().valid, &value_4);
        ASSERT_EQ(other_data_islist.back().valid, &value_6);
        ASSERT_EQ(other_data_islist.size(), 2);
    }
}

TEST(intrusive_list_tests, remove_if__lambda__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "22");
        data_islist.emplace_back(value_3, "3");
        data_islist.emplace_back(value_4, "44");
        data_islist.emplace_back(value_5, "5");
        data_islist.emplace_back(value_6, "66");
        data_islist.remove_if([](const data_silist_node& node) { return node.text.size() == 2; });
        ASSERT_EQ(data_islist.size(), 3);
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
        {
            value_pointers.push_back(item.valid);
            ASSERT_TRUE(*item.valid);
        }
        std::vector<const bool*> expected_pointers{ &value_1, &value_3, &value_5 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_FALSE(value_2);
        ASSERT_FALSE(value_4);
        ASSERT_FALSE(value_6);
    }
}

TEST(intrusive_list_tests, std_erase_if__lambda__no_exception)
{
    bool value_1 = false;
    bool value_2 = false;
    bool value_3 = false;
    bool value_4 = false;
    bool value_5 = false;
    bool value_6 = false;
    {
        itru::sharable_intrusive_list<data_silist_node> data_islist;
        data_islist.emplace_back(value_1, "1");
        data_islist.emplace_back(value_2, "22");
        data_islist.emplace_back(value_3, "3");
        data_islist.emplace_back(value_4, "44");
        data_islist.emplace_back(value_5, "5");
        data_islist.emplace_back(value_6, "66");
        std::erase_if(data_islist, [](const data_silist_node& node) { return node.text.size() == 2; });
        ASSERT_EQ(data_islist.size(), 3);
        std::vector<const bool*> value_pointers;
        for (const auto& item : data_islist)
        {
            value_pointers.push_back(item.valid);
            ASSERT_TRUE(*item.valid);
        }
        std::vector<const bool*> expected_pointers{ &value_1, &value_3, &value_5 };
        ASSERT_EQ(value_pointers, expected_pointers);
        ASSERT_FALSE(value_2);
        ASSERT_FALSE(value_4);
        ASSERT_FALSE(value_6);
    }
}
