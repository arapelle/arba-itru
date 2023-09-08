#include <arba/itru/intrusive_shared_ptr.hpp>
#include <gtest/gtest.h>

// data_with_custom_counter

struct data_with_custom_counter
{
    int counter = 0;
    std::string text;
    bool* valid = nullptr;

    explicit data_with_custom_counter(bool& valid) : valid(&valid) { valid = true; }
    ~data_with_custom_counter() { *valid = false; }
};

// counter manager functions for data_with_custom_counter

void intrusive_shared_ptr_add_ref(data_with_custom_counter* ptr) noexcept
{
    ++(ptr->counter);
}

void intrusive_shared_ptr_release(data_with_custom_counter* ptr) noexcept
{
    if (--(ptr->counter); ptr->counter == 0)
        delete ptr;
}

// tests:

TEST(isptr_with_custom_counter_tests, test_iptr_empty_constructor)
{
    itru::intrusive_shared_ptr<data_with_custom_counter> iptr;
    ASSERT_EQ(iptr.get(), nullptr);
}

TEST(isptr_with_custom_counter_tests, test_iptr_constructor_ptr)
{
    bool valid = false;
    {
        data_with_custom_counter* data_ptr = new data_with_custom_counter(valid);
        itru::intrusive_shared_ptr<data_with_custom_counter> iptr(data_ptr);
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(isptr_with_custom_counter_tests, test_iptr_copy_constructor)
{
    bool valid = false;
    {
        data_with_custom_counter* data_ptr = new data_with_custom_counter(valid);
        itru::intrusive_shared_ptr<data_with_custom_counter> iptr(data_ptr);
        {
            itru::intrusive_shared_ptr<data_with_custom_counter> jptr(iptr);
            ASSERT_EQ(iptr.get(), data_ptr);
            ASSERT_EQ(iptr->counter, 2);
            ASSERT_EQ(jptr.get(), data_ptr);
            ASSERT_EQ(jptr->counter, 2);
            ASSERT_TRUE(valid);
        }
        ASSERT_EQ(iptr.get(), data_ptr);
        ASSERT_EQ(iptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}

TEST(isptr_with_custom_counter_tests, test_iptr_move_constructor)
{
    bool valid = false;
    data_with_custom_counter* data_ptr = new data_with_custom_counter(valid);
    itru::intrusive_shared_ptr<data_with_custom_counter> iptr(data_ptr);
    {
        itru::intrusive_shared_ptr<data_with_custom_counter> jptr(std::move(iptr));
        ASSERT_EQ(iptr.get(), nullptr);
        ASSERT_EQ(jptr.get(), data_ptr);
        ASSERT_EQ(jptr->counter, 1);
        ASSERT_TRUE(valid);
    }
    ASSERT_FALSE(valid);
}
