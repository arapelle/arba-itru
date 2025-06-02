#pragma once

#include <arba/meta/policy/thread_policy.hpp>
#include <arba/meta/type_traits/integer_n.hpp>

inline namespace arba
{
namespace itru
{

class intrusive_type_base
{
private:
    inline ~intrusive_type_base() = default;

    template <unsigned counter_bitsize, meta::ThreadPolicy th_policy>
        requires(sizeof(meta::uint_n_t<counter_bitsize, th_policy>) <= 8)
    friend class intrusive_ref_counter;

    template <unsigned counter_bitsize, meta::ThreadPolicy th_policy>
        requires(sizeof(meta::uint_n_t<counter_bitsize, th_policy>) < 8
                 || std::is_same_v<th_policy, meta::thread_unsafe_t>)
    friend class intrusive_ref_counters;
};

template <unsigned counter_bitsize = 32, meta::ThreadPolicy th_policy = meta::thread_safe_t>
    requires(sizeof(meta::uint_n_t<counter_bitsize, th_policy>) <= 8)
class intrusive_ref_counter;

template <unsigned counter_bitsize = 32, meta::ThreadPolicy th_policy = meta::thread_safe_t>
    requires(sizeof(meta::uint_n_t<counter_bitsize, th_policy>) < 8 || std::is_same_v<th_policy, meta::thread_unsafe_t>)
class intrusive_ref_counters;

template <unsigned counter_bitsize>
class intrusive_ref_counter<counter_bitsize, meta::thread_safe_t> : public intrusive_type_base
{
private:
    using atomic_ref_counter_type = meta::uint_n_t<counter_bitsize, meta::thread_safe_t>;

public:
    intrusive_ref_counter() noexcept : ref_counter_(0) {}

    explicit intrusive_ref_counter(const intrusive_ref_counter&) noexcept : ref_counter_(0) {}

    intrusive_ref_counter& operator=(const intrusive_ref_counter&) noexcept { return *this; }

    [[nodiscard]] inline unsigned int use_count() const noexcept { return ref_counter_.load(); }

    inline static void increment_use_counter(intrusive_ref_counter* ptr) noexcept { ptr->ref_counter_.fetch_add(1); }

    [[nodiscard]] inline static bool decrement_use_counter(intrusive_ref_counter* ptr) noexcept
    {
        return ptr->ref_counter_.fetch_sub(1) == 1;
    }

protected:
    ~intrusive_ref_counter() = default;

private:
    atomic_ref_counter_type ref_counter_ = 0;
};

template <unsigned counter_bitsize>
class intrusive_ref_counter<counter_bitsize, meta::thread_unsafe_t> : public intrusive_type_base
{
private:
    using ref_counter_type = meta::uint_n_t<counter_bitsize, meta::thread_unsafe_t>;

public:
    intrusive_ref_counter() noexcept {}

    explicit intrusive_ref_counter(const intrusive_ref_counter&) noexcept : intrusive_ref_counter() {}

    intrusive_ref_counter& operator=(const intrusive_ref_counter&) noexcept { return *this; }

    [[nodiscard]] inline unsigned int use_count() const noexcept { return ref_counter_; }

    inline static void increment_use_counter(intrusive_ref_counter* ptr) noexcept { ++(ptr->ref_counter_); }

    [[nodiscard]] inline static bool decrement_use_counter(intrusive_ref_counter* ptr) noexcept
    {
        return (--(ptr->ref_counter_)) == 0;
    }

protected:
    ~intrusive_ref_counter() = default;

private:
    ref_counter_type ref_counter_ = 0;
};

template <unsigned counter_bitsize>
class intrusive_ref_counters<counter_bitsize, meta::thread_unsafe_t>
    : public intrusive_ref_counter<counter_bitsize, meta::thread_unsafe_t>
{
private:
    using ref_counter_type = meta::uint_n_t<counter_bitsize, meta::thread_unsafe_t>;

public:
    intrusive_ref_counters() noexcept {}

    explicit intrusive_ref_counters(const intrusive_ref_counters&) noexcept : intrusive_ref_counters() {}

    intrusive_ref_counters& operator=(const intrusive_ref_counters&) noexcept { return *this; }

    [[nodiscard]] inline unsigned int latent_count() const noexcept { return latent_counter_; }

    [[nodiscard]] inline static bool decrement_use_counter(intrusive_ref_counters* ptr) noexcept
    {
        return intrusive_ref_counter<counter_bitsize, meta::thread_unsafe_t>::decrement_use_counter(ptr)
               && ptr->latent_counter_ == 0;
    }

    [[nodiscard]] inline static bool lock_use_counter(intrusive_ref_counters* ptr) noexcept
    {
        intrusive_ref_counter<counter_bitsize, meta::thread_unsafe_t>::increment_use_counter(ptr);
        return true;
    }

    inline static void increment_latent_counter(intrusive_ref_counters* ptr) noexcept { ++(ptr->latent_counter_); }

    [[nodiscard]] inline static bool decrement_latent_counter(intrusive_ref_counters* ptr) noexcept
    {
        return (--(ptr->latent_counter_)) == 0 && ptr->use_count() == 0;
    }

protected:
    ~intrusive_ref_counters() = default;

private:
    ref_counter_type latent_counter_ = 0;
};

template <unsigned counter_bitsize>
class intrusive_ref_counters<counter_bitsize, meta::thread_safe_t> : public intrusive_type_base
{
private:
    using atomic_ref_counter_type = meta::uint_n_t<counter_bitsize * 2, meta::thread_safe_t>;
    using ref_counter_type = typename atomic_ref_counter_type::value_type;
    static constexpr ref_counter_type use_count_mask = meta::uint_n_t<counter_bitsize, meta::thread_unsafe_t>(~0);
    static constexpr ref_counter_type latent_count_mask = use_count_mask << counter_bitsize;

public:
    intrusive_ref_counters() noexcept : ref_counter_(0) {}

    explicit intrusive_ref_counters(const intrusive_ref_counters&) noexcept : ref_counter_(0) {}

    intrusive_ref_counters& operator=(const intrusive_ref_counters&) noexcept { return *this; }

    [[nodiscard]] inline unsigned int use_count() const noexcept { return ref_counter_.load() & use_count_mask; }
    [[nodiscard]] inline unsigned int latent_count() const noexcept
    {
        return (ref_counter_.load() & latent_count_mask) >> counter_bitsize;
    }

    inline static void increment_use_counter(intrusive_ref_counters* ptr) noexcept { ptr->ref_counter_.fetch_add(1); }

    [[nodiscard]] inline static bool decrement_use_counter(intrusive_ref_counters* ptr) noexcept
    {
        return ptr->ref_counter_.fetch_sub(1) == 1;
    }

    [[nodiscard]] inline static bool lock_use_counter(intrusive_ref_counters* ptr) noexcept
    {
        ref_counter_type counter = ptr->ref_counter_;
        do
        {
            if ((counter & use_count_mask) == 0)
                return false;
        } while (!ptr->ref_counter_.compare_exchange_strong(counter, counter + 1));
        return true;
    }

    inline static void increment_latent_counter(intrusive_ref_counters* ptr) noexcept
    {
        ptr->ref_counter_.fetch_add(1LL << counter_bitsize);
    }

    [[nodiscard]] inline static bool decrement_latent_counter(intrusive_ref_counters* ptr) noexcept
    {
        return (ptr->ref_counter_.fetch_sub(1LL << counter_bitsize)) == 1LL << counter_bitsize;
    }

protected:
    ~intrusive_ref_counters() = default;

private:
    atomic_ref_counter_type ref_counter_ = 0;
};

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
void shared_intrusive_ptr_add_ref(ElementType* ptr) noexcept
{
    ElementType::increment_use_counter(ptr);
}

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
void shared_intrusive_ptr_release(ElementType* ptr) noexcept
{
    if (ElementType::decrement_use_counter(ptr))
        delete ptr;
}

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
void weak_intrusive_ptr_add_ref(ElementType* ptr) noexcept
{
    ElementType::increment_latent_counter(ptr);
}

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
             && requires(ElementType* ptr) {
                    ElementType::decrement_latent_counter(ptr);
                }
void weak_intrusive_ptr_release(ElementType* ptr) noexcept
{
    if (ElementType::decrement_latent_counter(ptr))
        delete ptr;
}

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
[[nodiscard]] bool weak_intrusive_ptr_lock(ElementType* ptr) noexcept
{
    return ElementType::lock_use_counter(ptr);
}

template <class ElementType>
    requires std::is_base_of_v<intrusive_type_base, ElementType>
[[nodiscard]] std::size_t weak_intrusive_ptr_use_count(ElementType* ptr) noexcept
{
    return ptr->use_count();
}

} // namespace itru
} // namespace arba
