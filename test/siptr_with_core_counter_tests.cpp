#include "data_with_core_counter.hpp"
#include <arba/itru/shared_intrusive_ptr.hpp>
#include <arba/itru/concept/sharable_intrusive.hpp>

#include <gtest/gtest.h>

#include <set>
#include <unordered_set>

// TEST(siptr_with_core_counter_tests, SharableIntrusive__valid_type__true)
static_assert(itru::SharableIntrusive<data_with_ircnt>);

TEST(siptr_with_core_counter_tests, test_iptr_empty_constructor)
{
    itru::shared_intrusive_ptr<data_with_ircnt> iptr;
    ASSERT_EQ(iptr.get(), nullptr);
}

TEST(siptr_with_core_counter_tests, test_iptr_constructor_ptr)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_copy_constructor)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            itru::shared_intrusive_ptr<data_with_ircnt> jptr(iptr);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_copy_constructor_other)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            itru::shared_intrusive_ptr<base_with_ircnt> jptr(iptr);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_move_constructor)
{
    bool valid = false;
    data_with_ircnt* data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
    {
        itru::shared_intrusive_ptr<data_with_ircnt> jptr(std::move(iptr));
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_EQ(jptr.get(), data_ptr);
        ASSERT_EQ(jptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_move_constructor_other)
{
    bool valid = false;
    data_with_ircnt* data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
    {
        itru::shared_intrusive_ptr<base_with_ircnt> jptr(std::move(iptr));
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_EQ(jptr.get(), data_ptr);
        ASSERT_EQ(jptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_copy_assignment)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            bool valid_2 = false;
            itru::shared_intrusive_ptr<data_with_ircnt> jptr(new data_with_ircnt(valid_2));
            ASSERT_TRUE(valid_2);
            jptr = iptr;
            ASSERT_FALSE(valid_2);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_copy_assignment_other)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            bool valid_2 = false;
            itru::shared_intrusive_ptr<base_with_ircnt> jptr(new data_with_ircnt(valid_2));
            ASSERT_TRUE(valid_2);
            jptr = iptr;
            ASSERT_FALSE(valid_2);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->use_count(), 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_move_assignment)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            bool valid_2 = false;
            itru::shared_intrusive_ptr<data_with_ircnt> jptr(new data_with_ircnt(valid_2));
            ASSERT_TRUE(valid_2);
            jptr = std::move(iptr);
            ASSERT_FALSE(valid_2);
            ASSERT_EQ(iptr.get(), nullptr);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 1);
            ASSERT_TRUE(valid);
        }
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_move_assignment_other)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        {
            bool valid_2 = false;
            itru::shared_intrusive_ptr<base_with_ircnt> jptr(new data_with_ircnt(valid_2));
            ASSERT_TRUE(valid_2);
            jptr = std::move(iptr);
            ASSERT_FALSE(valid_2);
            ASSERT_EQ(iptr.get(), nullptr);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->use_count(), 1);
            ASSERT_TRUE(valid);
        }
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

//----------------------------------------------------------------------

TEST(siptr_with_core_counter_tests, test_iptr_release)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        data_with_ircnt* d_ptr = iptr.release();
        ASSERT_EQ(d_ptr, data_ptr);
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_reset_nullptr)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        iptr.reset();
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_reset_other_ptr)
{
    bool valid = false;
    bool other_valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        data_with_ircnt* other_data_ptr = new data_with_ircnt(other_valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        iptr.reset(other_data_ptr);
        ASSERT_EQ(iptr.get(), other_data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(other_valid);
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
    ASSERT_FALSE(other_valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_deref)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        data_with_ircnt& data_ref = *data_ptr;
        ASSERT_EQ(&data_ref, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_deref_const)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        const itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        const data_with_ircnt& data_ref = *data_ptr;
        ASSERT_EQ(&data_ref, data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_arrow)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid, "arba::core");
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        ASSERT_EQ(iptr->text, "arba::core");
        iptr->text = "arba_core";
        ASSERT_EQ(&iptr->text, &data_ptr->text);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_arrow_const)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid, "arba::core");
        const itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        ASSERT_EQ(iptr->text, "arba::core");
        iptr->text = "arba_core";
        ASSERT_EQ(&iptr->text, &data_ptr->text);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_cast_to_bool)
{
    bool valid = false;
    {
        data_with_ircnt* data_ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
        bool test_bool = iptr;
        ASSERT_TRUE(test_bool);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->use_count(), 1);
        ASSERT_TRUE(valid);
        iptr.reset();
        test_bool = iptr;
        ASSERT_FALSE(test_bool);
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_swap)
{
    bool valid = false;
    data_with_ircnt* data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
    data_with_ircnt* other_data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> jptr(other_data_ptr);
    iptr.swap(jptr);
    ASSERT_EQ(iptr.get(), other_data_ptr);
    ASSERT_EQ(iptr->use_count(), 1);
    ASSERT_TRUE(valid);
    ASSERT_EQ(jptr.get(), data_ptr);
    ASSERT_EQ(jptr->use_count(), 1);
    ASSERT_TRUE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_eq)
{
    bool valid = false;
    data_with_ircnt* data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
    itru::shared_intrusive_ptr<data_with_ircnt> jptr(data_ptr);
    ASSERT_EQ(iptr, jptr);
    ASSERT_EQ(iptr->use_count(), 2);
}

TEST(siptr_with_core_counter_tests, test_iptr_operator_ne)
{
    bool valid = false;
    data_with_ircnt* data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> iptr(data_ptr);
    data_with_ircnt* other_data_ptr = new data_with_ircnt(valid);
    itru::shared_intrusive_ptr<data_with_ircnt> jptr(other_data_ptr);
    ASSERT_NE(iptr, jptr);
}

TEST(siptr_with_core_counter_tests, test_iptr_hash)
{
    bool valid = true;
    {
        data_with_ircnt* ptr = new data_with_ircnt(valid);
        itru::shared_intrusive_ptr<data_with_ircnt> iptr(ptr);
        ASSERT_EQ(std::hash<itru::shared_intrusive_ptr<data_with_ircnt>>{}(iptr), std::hash<data_with_ircnt*>{}(ptr));
    }
    ASSERT_FALSE(valid);
}

TEST(siptr_with_core_counter_tests, test_iptr_set_and_uset)
{
    bool b_set = true;
    bool b_uset = true;
    {
        std::set<itru::shared_intrusive_ptr<data_with_ircnt>> iptr_set;
        iptr_set.insert(itru::shared_intrusive_ptr<data_with_ircnt>(new data_with_ircnt(b_set)));
        std::unordered_set<itru::shared_intrusive_ptr<data_with_ircnt>> iptr_uset;
        iptr_uset.insert(itru::shared_intrusive_ptr<data_with_ircnt>(new data_with_ircnt(b_uset)));
    }
    ASSERT_FALSE(b_set);
    ASSERT_FALSE(b_uset);
}
