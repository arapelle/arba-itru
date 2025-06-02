#include "data_with_core_counter.hpp"
#include <arba/itru/weak_intrusive_ptr.hpp>
#include <arba/itru/concept/latent_intrusive.hpp>

#include <gtest/gtest.h>

#include <set>
#include <unordered_set>

// TEST(wiptr_with_core_counter_tests, LatentIntrusive__valid_type__true)
static_assert(itru::LatentIntrusive<data_with_ircnts>);

// TEST(wiptr_with_core_counter_tests, LatentIntrusive__invalid_type__false)
static_assert(!itru::LatentIntrusive<data_with_ircnt>);

TEST(wiptr_with_core_counter_tests, test_wiptr_empty_constructor)
{
    itru::weak_intrusive_ptr<data_with_ircnts> iptr;
    ASSERT_EQ(iptr.use_count(), 0);
    ASSERT_TRUE(iptr.expired());
}

TEST(wiptr_with_core_counter_tests, test_wiptr_constructor_sptr)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        ASSERT_EQ(siptr.get(), data_ptr);
        ASSERT_EQ(siptr->use_count(), 1);
        ASSERT_TRUE(valid);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        ASSERT_EQ(wiptr.use_count(), 1);
        ASSERT_FALSE(wiptr.expired());
        siptr.release();
        ASSERT_EQ(wiptr.use_count(), 0);
        ASSERT_TRUE(wiptr.expired());
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_constructor_sptr_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<base_with_ircnts> wiptr(siptr);
        ASSERT_EQ(wiptr.use_count(), 1);
        ASSERT_FALSE(wiptr.expired());
        siptr.release();
        ASSERT_EQ(wiptr.use_count(), 0);
        ASSERT_TRUE(wiptr.expired());
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_lock_success)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        ASSERT_EQ(siptr.get(), data_ptr);
        ASSERT_EQ(siptr->use_count(), 1);
        ASSERT_EQ(siptr->text, "text");
        ASSERT_TRUE(valid);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        ASSERT_EQ(wiptr.use_count(), 1);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr_2 = wiptr.lock();
        ASSERT_TRUE(siptr_2);
        ASSERT_EQ(siptr_2->use_count(), 2);
        ASSERT_EQ(siptr_2->text, "text");
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_lock_failure)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        ASSERT_EQ(wiptr.use_count(), 1);
        siptr.release();
        itru::shared_intrusive_ptr<data_with_ircnts> siptr_2 = wiptr.lock();
        ASSERT_FALSE(siptr_2);
        ASSERT_EQ(siptr_2.get(), nullptr);
        ASSERT_TRUE(wiptr.expired());
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_operator_eq_ne)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_2(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_3;
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_4;
        ASSERT_EQ(wiptr.lock(), siptr);
        ASSERT_EQ(wiptr, wiptr_2);
        ASSERT_NE(wiptr, wiptr_3);
        ASSERT_NE(wiptr_2, wiptr_3);
        ASSERT_EQ(wiptr_3, wiptr_4);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_reset)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_null;
        siptr.release();
        ASSERT_TRUE(valid);
        ASSERT_NE(wiptr, wiptr_null);
        wiptr.reset();
        ASSERT_FALSE(valid);
        ASSERT_EQ(wiptr, wiptr_null);
        wiptr_null.reset();
        ASSERT_EQ(wiptr, wiptr_null);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_swap)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_alpha(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_data(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_beta;
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_null;
        ASSERT_EQ(wiptr_alpha, wiptr_data);
        ASSERT_EQ(wiptr_beta, wiptr_null);
        ASSERT_NE(wiptr_alpha, wiptr_beta);
        ASSERT_NE(wiptr_data, wiptr_beta);
        wiptr_alpha.swap(wiptr_beta);
        ASSERT_NE(wiptr_alpha, wiptr_beta);
        ASSERT_EQ(wiptr_data, wiptr_beta);
        ASSERT_EQ(wiptr_alpha, wiptr_null);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_copy_constructor)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_2(wiptr);
        ASSERT_EQ(wiptr_2.lock().get(), data_ptr);
        siptr.release();
        wiptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_copy_constructor_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<base_with_ircnts> wiptr_2(wiptr);
        ASSERT_EQ(wiptr_2.lock(), siptr);
        siptr.release();
        wiptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_move_constructor)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_2(std::move(wiptr));
        ASSERT_EQ(wiptr_2.lock(), siptr);
        ASSERT_EQ(wiptr.lock().get(), nullptr);
        siptr.release();
        wiptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_move_constructor_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<base_with_ircnts> wiptr_2(std::move(wiptr));
        ASSERT_EQ(wiptr_2.lock(), siptr);
        ASSERT_EQ(wiptr.lock().get(), nullptr);
        siptr.release();
        wiptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_assignement)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_2;
        wiptr_2 = wiptr;
        ASSERT_EQ(wiptr_2.lock().get(), data_ptr);
        siptr.release();
        wiptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_assignement_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<base_with_ircnts> wiptr_2;
        wiptr_2 = wiptr;
        ASSERT_EQ(wiptr_2.lock().get(), data_ptr);
        siptr.release();
        wiptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_move_assignement)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr_2;
        wiptr_2 = std::move(wiptr);
        ASSERT_EQ(wiptr_2.lock(), siptr);
        ASSERT_EQ(wiptr.lock().get(), nullptr);
        siptr.release();
        wiptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_move_assignement_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr(siptr);
        itru::weak_intrusive_ptr<base_with_ircnts> wiptr_2;
        wiptr_2 = std::move(wiptr);
        ASSERT_EQ(wiptr_2.lock(), siptr);
        ASSERT_EQ(wiptr.lock().get(), nullptr);
        siptr.release();
        wiptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(wiptr_with_core_counter_tests, test_wiptr_assignement_sptr_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::shared_intrusive_ptr<data_with_ircnts> siptr(data_ptr);
        itru::weak_intrusive_ptr<data_with_ircnts> wiptr;
        wiptr = siptr;
        ASSERT_EQ(wiptr.lock(), siptr);
        siptr.release();
        ASSERT_TRUE(wiptr.expired());
        ASSERT_TRUE(valid);
        wiptr.reset();
    }
    ASSERT_FALSE(valid);
}
