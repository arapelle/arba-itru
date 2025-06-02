#include <arba/itru/intrusive_ref_counter.hpp>

#include <gtest/gtest.h>

template <unsigned counter_bitsize, meta::ThreadPolicy th_policy>
class intrusive_ref_counter : public itru::intrusive_ref_counter<counter_bitsize, th_policy>
{
private:
    using base_ = itru::intrusive_ref_counter<counter_bitsize, th_policy>;

public:
    explicit intrusive_ref_counter(bool& bval)
    {
        bval_ptr = &bval;
        bval = true;
    }

    intrusive_ref_counter(const intrusive_ref_counter& other, bool& bval) : base_(other)
    {
        bval_ptr = &bval;
        bval = true;
    }

    ~intrusive_ref_counter()
    {
        *bval_ptr = false;
        bval_ptr = nullptr;
    }

    intrusive_ref_counter& operator=(const intrusive_ref_counter& other)
    {
        static_cast<base_&>(*this) = other;
        return *this;
    }

private:
    bool* bval_ptr = nullptr;
};

template <class ircnt_type>
struct ircnt_tests
{
    static void test_ircnt()
    {
        bool valid = false;
        ircnt_type* ircnt_ptr = new ircnt_type(valid);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnt_ptr->use_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr);
        ASSERT_EQ(ircnt_ptr->use_count(), 1);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr);
        ASSERT_EQ(ircnt_ptr->use_count(), 2);
        itru::shared_intrusive_ptr_release(ircnt_ptr);
        ASSERT_TRUE(valid);
        ASSERT_EQ(ircnt_ptr->use_count(), 1);
        itru::shared_intrusive_ptr_release(ircnt_ptr);
        ASSERT_FALSE(valid);
    }

    static void test_ircnt_copy()
    {
        bool valid = false;
        bool valid_2 = false;
        ircnt_type* ircnt_ptr = new ircnt_type(valid);
        ircnt_type* ircnt_ptr_2 = new ircnt_type(*ircnt_ptr, valid_2);
        ASSERT_TRUE(valid);
        ASSERT_TRUE(valid_2);
        ASSERT_EQ(ircnt_ptr->use_count(), 0);
        ASSERT_EQ(ircnt_ptr_2->use_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr_2);
        itru::shared_intrusive_ptr_release(ircnt_ptr_2);
        ASSERT_TRUE(valid);
        ASSERT_FALSE(valid_2);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr);
        itru::shared_intrusive_ptr_release(ircnt_ptr);
        ASSERT_FALSE(valid_2);
    }

    static void test_ircnt_assignment()
    {
        bool valid = false;
        bool valid_2 = false;
        ircnt_type* ircnt_ptr = new ircnt_type(valid);
        ircnt_type* ircnt_ptr_2 = new ircnt_type(valid_2);
        *ircnt_ptr_2 = *ircnt_ptr;
        ASSERT_TRUE(valid);
        ASSERT_TRUE(valid_2);
        ASSERT_EQ(ircnt_ptr->use_count(), 0);
        ASSERT_EQ(ircnt_ptr_2->use_count(), 0);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr_2);
        itru::shared_intrusive_ptr_release(ircnt_ptr_2);
        ASSERT_TRUE(valid);
        ASSERT_FALSE(valid_2);
        itru::shared_intrusive_ptr_add_ref(ircnt_ptr);
        itru::shared_intrusive_ptr_release(ircnt_ptr);
        ASSERT_FALSE(valid_2);
    }
};

using ircnt_64_mt = intrusive_ref_counter<64, meta::thread_safe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_64_mt)
{
    ircnt_tests<ircnt_64_mt>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_64_mt_copy)
{
    ircnt_tests<ircnt_64_mt>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_64_mt_assignment)
{
    ircnt_tests<ircnt_64_mt>::test_ircnt_assignment();
}

using ircnt_64_st = intrusive_ref_counter<64, meta::thread_unsafe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_64_st)
{
    ircnt_tests<ircnt_64_st>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_64_st_copy)
{
    ircnt_tests<ircnt_64_st>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_64_st_assignment)
{
    ircnt_tests<ircnt_64_st>::test_ircnt_assignment();
}

using ircnt_32_mt = intrusive_ref_counter<32, meta::thread_safe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_32_mt)
{
    ircnt_tests<ircnt_32_mt>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_32_mt_copy)
{
    ircnt_tests<ircnt_32_mt>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_32_mt_assignment)
{
    ircnt_tests<ircnt_32_mt>::test_ircnt_assignment();
}

using ircnt_32_st = intrusive_ref_counter<32, meta::thread_unsafe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_32_st)
{
    ircnt_tests<ircnt_32_st>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_32_st_copy)
{
    ircnt_tests<ircnt_32_st>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_32_st_assignment)
{
    ircnt_tests<ircnt_32_st>::test_ircnt_assignment();
}

using ircnt_16_mt = intrusive_ref_counter<16, meta::thread_safe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_16_mt)
{
    ircnt_tests<ircnt_16_mt>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_16_mt_copy)
{
    ircnt_tests<ircnt_16_mt>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_16_mt_assignment)
{
    ircnt_tests<ircnt_16_mt>::test_ircnt_assignment();
}

using ircnt_16_st = intrusive_ref_counter<16, meta::thread_unsafe_t>;

TEST(intrusive_ref_counter_tests, test_ircnt_16_st)
{
    ircnt_tests<ircnt_16_st>::test_ircnt();
}

TEST(intrusive_ref_counter_tests, test_ircnt_16_st_copy)
{
    ircnt_tests<ircnt_16_st>::test_ircnt_copy();
}

TEST(intrusive_ref_counter_tests, test_ircnt_16_st_assignment)
{
    ircnt_tests<ircnt_16_st>::test_ircnt_assignment();
}
