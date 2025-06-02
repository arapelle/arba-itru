#include <arba/itru/intrusive_ref_counter.hpp>

#include <gtest/gtest.h>

template <unsigned counter_bitsize, meta::ThreadPolicy th_policy>
class intrusive_ref_counters : public itru::intrusive_ref_counters<counter_bitsize, th_policy>
{
private:
    using base_ = itru::intrusive_ref_counters<counter_bitsize, th_policy>;

public:
    explicit intrusive_ref_counters(bool& bval)
    {
        bval_ptr = &bval;
        bval = true;
    }

    intrusive_ref_counters(const intrusive_ref_counters& other, bool& bval) : base_(other)
    {
        bval_ptr = &bval;
        bval = true;
    }

    ~intrusive_ref_counters()
    {
        *bval_ptr = false;
        bval_ptr = nullptr;
    }

    intrusive_ref_counters& operator=(const intrusive_ref_counters& other)
    {
        static_cast<base_&>(*this) = other;
        return *this;
    }

private:
    bool* bval_ptr = nullptr;
};

template <class ircnts_type>
struct ircnts_tests
{
    static void test_ircnts_delete_when_use_count_is_zero()
    {
        bool valid = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        ASSERT_EQ(ircnts_ptr->latent_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        ASSERT_EQ(ircnts_ptr->use_count(), 1);
        ASSERT_EQ(ircnts_ptr->latent_count(), 0);
        itru::weak_intrusive_ptr_add_ref(ircnts_ptr);
        ASSERT_EQ(ircnts_ptr->use_count(), 1);
        ASSERT_EQ(ircnts_ptr->latent_count(), 1);
        itru::weak_intrusive_ptr_release(ircnts_ptr);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnts_ptr->use_count(), 1);
        ASSERT_EQ(ircnts_ptr->latent_count(), 0);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid);
    }

    static void test_ircnts_delete_when_latent_count_is_zero()
    {
        bool valid = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        itru::weak_intrusive_ptr_add_ref(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        ASSERT_EQ(ircnts_ptr->latent_count(), 1);
        itru::weak_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid);
    }

    static void test_ircnts_lock_success()
    {
        bool valid = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        bool lock = itru::weak_intrusive_ptr_lock(ircnts_ptr);
        ASSERT_TRUE(lock);
        itru::weak_intrusive_ptr_add_ref(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        ASSERT_EQ(ircnts_ptr->latent_count(), 1);
        itru::weak_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid);
    }

    static void test_ircnts_lock_fail()
    {
        bool valid = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        bool lock = itru::weak_intrusive_ptr_lock(ircnts_ptr);
        ASSERT_FALSE(lock);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid);
    }

    static void test_ircnts_copy()
    {
        bool valid = false;
        bool valid_2 = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        ircnts_type* ircnts_ptr_2 = new ircnts_type(*ircnts_ptr, valid_2);
        ASSERT_TRUE(valid);
        ASSERT_TRUE(valid_2);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        ASSERT_EQ(ircnts_ptr->latent_count(), 0);
        ASSERT_EQ(ircnts_ptr_2->use_count(), 0);
        ASSERT_EQ(ircnts_ptr_2->latent_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr_2);
        itru::weak_intrusive_ptr_add_ref(ircnts_ptr_2);
        itru::shared_intrusive_ptr_release(ircnts_ptr_2);
        ASSERT_TRUE(valid_2);
        itru::weak_intrusive_ptr_release(ircnts_ptr_2);
        ASSERT_TRUE(valid);
        ASSERT_FALSE(valid_2);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid_2);
    }

    static void test_ircnts_assignment()
    {
        bool valid = false;
        bool valid_2 = false;
        ircnts_type* ircnts_ptr = new ircnts_type(valid);
        ircnts_type* ircnts_ptr_2 = new ircnts_type(valid_2);
        *ircnts_ptr_2 = *ircnts_ptr;
        ASSERT_TRUE(valid);
        ASSERT_TRUE(valid_2);
        ASSERT_EQ(ircnts_ptr->use_count(), 0);
        ASSERT_EQ(ircnts_ptr->latent_count(), 0);
        ASSERT_EQ(ircnts_ptr_2->use_count(), 0);
        ASSERT_EQ(ircnts_ptr_2->latent_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr_2);
        itru::weak_intrusive_ptr_add_ref(ircnts_ptr_2);
        itru::shared_intrusive_ptr_release(ircnts_ptr_2);
        ASSERT_TRUE(valid_2);
        itru::weak_intrusive_ptr_release(ircnts_ptr_2);
        ASSERT_TRUE(valid);
        ASSERT_FALSE(valid_2);
        itru::shared_intrusive_ptr_add_ref(ircnts_ptr);
        itru::shared_intrusive_ptr_release(ircnts_ptr);
        ASSERT_FALSE(valid_2);
    }
};

using ircnt_64_st = intrusive_ref_counters<64, meta::thread_unsafe_t>;

TEST(intrusive_ref_counters_tests, test_ircnts_64_st)
{
    ircnts_tests<ircnt_64_st>::test_ircnts_delete_when_use_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_64_st_2)
{
    ircnts_tests<ircnt_64_st>::test_ircnts_delete_when_latent_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_64_st_3)
{
    ircnts_tests<ircnt_64_st>::test_ircnts_lock_success();
}

TEST(intrusive_ref_counters_tests, test_ircnts_64_st_copy)
{
    ircnts_tests<ircnt_64_st>::test_ircnts_copy();
}

TEST(intrusive_ref_counters_tests, test_ircnts_64_st_assignment)
{
    ircnts_tests<ircnt_64_st>::test_ircnts_assignment();
}

using ircnt_32_st = intrusive_ref_counters<32, meta::thread_unsafe_t>;

TEST(intrusive_ref_counters_tests, test_ircnts_32_st)
{
    ircnts_tests<ircnt_32_st>::test_ircnts_delete_when_use_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_st_2)
{
    ircnts_tests<ircnt_32_st>::test_ircnts_delete_when_latent_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_st_3)
{
    ircnts_tests<ircnt_32_st>::test_ircnts_lock_success();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_st_copy)
{
    ircnts_tests<ircnt_32_st>::test_ircnts_copy();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_st_assignment)
{
    ircnts_tests<ircnt_32_st>::test_ircnts_assignment();
}

using ircnt_32_mt = intrusive_ref_counters<32, meta::thread_safe_t>;

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_delete_when_use_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt_2)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_delete_when_latent_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt_3)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_lock_success();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt_3b)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_lock_fail();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt_copy)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_copy();
}

TEST(intrusive_ref_counters_tests, test_ircnts_32_mt_assignment)
{
    ircnts_tests<ircnt_32_mt>::test_ircnts_assignment();
}

using ircnt_16_st = intrusive_ref_counters<16, meta::thread_unsafe_t>;

TEST(intrusive_ref_counters_tests, test_ircnts_16_st)
{
    ircnts_tests<ircnt_16_st>::test_ircnts_delete_when_use_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_st_2)
{
    ircnts_tests<ircnt_16_st>::test_ircnts_delete_when_latent_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_st_3)
{
    ircnts_tests<ircnt_16_st>::test_ircnts_lock_success();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_st_copy)
{
    ircnts_tests<ircnt_16_st>::test_ircnts_copy();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_st_assignment)
{
    ircnts_tests<ircnt_16_st>::test_ircnts_assignment();
}

using ircnt_16_mt = intrusive_ref_counters<16, meta::thread_safe_t>;

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_delete_when_use_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt_2)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_delete_when_latent_count_is_zero();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt_3)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_lock_success();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt_3b)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_lock_fail();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt_copy)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_copy();
}

TEST(intrusive_ref_counters_tests, test_ircnts_16_mt_assignment)
{
    ircnts_tests<ircnt_16_mt>::test_ircnts_assignment();
}
