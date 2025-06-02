#include "data_with_core_counter.hpp"
#include <arba/itru/shared_intrusive_ptr.hpp>

#include <gtest/gtest.h>

TEST(make_siptr_tests, test_make_siptr)
{
    bool valid = false;
    {
        itru::shared_intrusive_ptr<data_with_ircnt> iptr =
            itru::make_shared_intrusive_ptr<data_with_ircnt>(valid, "make_iptr");
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "make_iptr");
    }
    ASSERT_FALSE(valid);
}

TEST(make_siptr_tests, test_make_siptr_with_move_arg)
{
    bool valid = false;
    {
        std::string str("move!");
        itru::shared_intrusive_ptr<data_with_ircnt_mc> iptr =
            itru::make_shared_intrusive_ptr<data_with_ircnt_mc>(valid, std::move(str));
        ASSERT_TRUE(valid);
        ASSERT_NE(iptr.get(), nullptr);
        ASSERT_EQ(iptr->text, "move!");
        ASSERT_EQ(str, "");
    }
    ASSERT_FALSE(valid);
}
