#pragma once

#include "intrusive_sharable.hpp"

inline namespace arba
{
namespace itru
{
template <class element_type>
void intrusive_weak_ptr_add_ref(element_type* ptr) noexcept;

template <class element_type>
void intrusive_weak_ptr_release(element_type* ptr) noexcept;

template <class element_type>
bool intrusive_weak_ptr_lock(element_type* ptr) noexcept;

template <class element_type>
std::size_t intrusive_weak_ptr_use_count(element_type* ptr) noexcept;

namespace concepts
{
template <class element_type>
concept IntrusiveLatent = intrusive_sharable<element_type> && requires(element_type* ptr) {
    intrusive_weak_ptr_add_ref(ptr);
    intrusive_weak_ptr_release(ptr);
    intrusive_weak_ptr_lock(ptr);
    intrusive_weak_ptr_use_count(ptr);
};

} // namespace concepts
} // namespace itru
} // namespace arba
