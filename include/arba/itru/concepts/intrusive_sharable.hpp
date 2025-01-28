#pragma once

inline namespace arba
{
namespace itru
{
template <class element_type>
void intrusive_shared_ptr_add_ref(element_type* ptr) noexcept;

template <class element_type>
void intrusive_shared_ptr_release(element_type* ptr) noexcept;

namespace concepts
{
template <class element_type>
concept IntrusiveSharable = requires(element_type* ptr) {
    intrusive_shared_ptr_add_ref(ptr);
    intrusive_shared_ptr_release(ptr);
};

} // namespace concepts
} // namespace itru
} // namespace arba
