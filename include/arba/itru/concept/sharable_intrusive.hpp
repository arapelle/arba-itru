#pragma once

inline namespace arba
{
namespace itru
{

template <class ElementType>
concept SharableIntrusive = requires(ElementType* ptr) {
    shared_intrusive_ptr_add_ref(ptr);
    shared_intrusive_ptr_release(ptr);
};

} // namespace itru
} // namespace arba
