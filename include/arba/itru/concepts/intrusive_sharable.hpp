#pragma once

inline namespace arba
{
namespace itru::concepts
{

template <class element_type>
void intrusive_shared_ptr_add_ref(element_type* ptr) noexcept;

template <class element_type>
void intrusive_shared_ptr_release(element_type* ptr) noexcept;

template <class element_type>
concept intrusive_sharable = requires(element_type* ptr) {
    intrusive_shared_ptr_add_ref(ptr);
    intrusive_shared_ptr_release(ptr);
};

} // namespace itru::concepts
} // namespace arba
