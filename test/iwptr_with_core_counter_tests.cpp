#include "data_with_core_counter.hpp"
#include <arba/itru/intrusive_weak_ptr.hpp>

#include <gtest/gtest.h>

#include <set>
#include <unordered_set>

TEST(iwptr_with_core_counter_tests, test_iwptr_empty_constructor)
{
    itru::intrusive_weak_ptr<data_with_ircnts> iptr;
    ASSERT_EQ(iptr.use_count(), 0);
    ASSERT_TRUE(iptr.expired());
}

TEST(iwptr_with_core_counter_tests, test_iwptr_constructor_sptr)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        ASSERT_EQ(isptr.get(), data_ptr);
        ASSERT_EQ(isptr->use_count(), 1);
        ASSERT_TRUE(valid);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        ASSERT_EQ(iwptr.use_count(), 1);
        ASSERT_FALSE(iwptr.expired());
        isptr.release();
        ASSERT_EQ(iwptr.use_count(), 0);
        ASSERT_TRUE(iwptr.expired());
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_constructor_sptr_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<base_with_ircnts> iwptr(isptr);
        ASSERT_EQ(iwptr.use_count(), 1);
        ASSERT_FALSE(iwptr.expired());
        isptr.release();
        ASSERT_EQ(iwptr.use_count(), 0);
        ASSERT_TRUE(iwptr.expired());
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_lock_success)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        ASSERT_EQ(isptr.get(), data_ptr);
        ASSERT_EQ(isptr->use_count(), 1);
        ASSERT_EQ(isptr->text, "text");
        ASSERT_TRUE(valid);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        ASSERT_EQ(iwptr.use_count(), 1);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr_2 = iwptr.lock();
        ASSERT_TRUE(isptr_2);
        ASSERT_EQ(isptr_2->use_count(), 2);
        ASSERT_EQ(isptr_2->text, "text");
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_lock_failure)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        ASSERT_EQ(iwptr.use_count(), 1);
        isptr.release();
        itru::intrusive_shared_ptr<data_with_ircnts> isptr_2 = iwptr.lock();
        ASSERT_FALSE(isptr_2);
        ASSERT_EQ(isptr_2.get(), nullptr);
        ASSERT_TRUE(iwptr.expired());
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_operator_eq_ne)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_2(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_3;
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_4;
        ASSERT_EQ(iwptr.lock(), isptr);
        ASSERT_EQ(iwptr, iwptr_2);
        ASSERT_NE(iwptr, iwptr_3);
        ASSERT_NE(iwptr_2, iwptr_3);
        ASSERT_EQ(iwptr_3, iwptr_4);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_reset)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_null;
        isptr.release();
        ASSERT_TRUE(valid);
        ASSERT_NE(iwptr, iwptr_null);
        iwptr.reset();
        ASSERT_FALSE(valid);
        ASSERT_EQ(iwptr, iwptr_null);
        iwptr_null.reset();
        ASSERT_EQ(iwptr, iwptr_null);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_swap)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid, "text");
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_alpha(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_data(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_beta;
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_null;
        ASSERT_EQ(iwptr_alpha, iwptr_data);
        ASSERT_EQ(iwptr_beta, iwptr_null);
        ASSERT_NE(iwptr_alpha, iwptr_beta);
        ASSERT_NE(iwptr_data, iwptr_beta);
        iwptr_alpha.swap(iwptr_beta);
        ASSERT_NE(iwptr_alpha, iwptr_beta);
        ASSERT_EQ(iwptr_data, iwptr_beta);
        ASSERT_EQ(iwptr_alpha, iwptr_null);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_copy_constructor)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_2(iwptr);
        ASSERT_EQ(iwptr_2.lock().get(), data_ptr);
        isptr.release();
        iwptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_copy_constructor_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<base_with_ircnts> iwptr_2(iwptr);
        ASSERT_EQ(iwptr_2.lock(), isptr);
        isptr.release();
        iwptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_move_constructor)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_2(std::move(iwptr));
        ASSERT_EQ(iwptr_2.lock(), isptr);
        ASSERT_EQ(iwptr.lock().get(), nullptr);
        isptr.release();
        iwptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_move_constructor_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<base_with_ircnts> iwptr_2(std::move(iwptr));
        ASSERT_EQ(iwptr_2.lock(), isptr);
        ASSERT_EQ(iwptr.lock().get(), nullptr);
        isptr.release();
        iwptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_assignement)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_2;
        iwptr_2 = iwptr;
        ASSERT_EQ(iwptr_2.lock().get(), data_ptr);
        isptr.release();
        iwptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_assignement_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<base_with_ircnts> iwptr_2;
        iwptr_2 = iwptr;
        ASSERT_EQ(iwptr_2.lock().get(), data_ptr);
        isptr.release();
        iwptr.reset();
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_move_assignement)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr_2;
        iwptr_2 = std::move(iwptr);
        ASSERT_EQ(iwptr_2.lock(), isptr);
        ASSERT_EQ(iwptr.lock().get(), nullptr);
        isptr.release();
        iwptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_move_assignement_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr(isptr);
        itru::intrusive_weak_ptr<base_with_ircnts> iwptr_2;
        iwptr_2 = std::move(iwptr);
        ASSERT_EQ(iwptr_2.lock(), isptr);
        ASSERT_EQ(iwptr.lock().get(), nullptr);
        isptr.release();
        iwptr_2.reset();
        ASSERT_FALSE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(iwptr_with_core_counter_tests, test_iwptr_assignement_sptr_u)
{
    bool valid = false;
    {
        data_with_ircnts* data_ptr = new data_with_ircnts(valid);
        itru::intrusive_shared_ptr<data_with_ircnts> isptr(data_ptr);
        itru::intrusive_weak_ptr<data_with_ircnts> iwptr;
        iwptr = isptr;
        ASSERT_EQ(iwptr.lock(), isptr);
        isptr.release();
        ASSERT_TRUE(iwptr.expired());
        ASSERT_TRUE(valid);
        iwptr.reset();
    }
    ASSERT_FALSE(valid);
}
