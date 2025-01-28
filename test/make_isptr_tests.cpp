#include "data_with_core_counter.hpp"
#include <arba/itru/intrusive_shared_ptr.hpp>

#include <gtest/gtest.h>

TEST(make_isptr_tests, test_make_isptr)
{
    bool valid = false;
    {
        itru::intrusive_shared_ptr<data_with_ircnt> iptr =
            itru::make_intrusive_shared_ptr<data_with_ircnt>(valid, "make_iptr");
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "make_iptr");
    }
    ASSERT_FALSE(valid);
}

TEST(make_isptr_tests, test_make_isptr_with_move_arg)
{
    bool valid = false;
    {
        std::string str("move!");
        itru::intrusive_shared_ptr<data_with_ircnt_mc> iptr =
            itru::make_intrusive_shared_ptr<data_with_ircnt_mc>(valid, std::move(str));
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "move!");
        ASSERT_EQ(str, "");
    }
    ASSERT_FALSE(valid);
}
