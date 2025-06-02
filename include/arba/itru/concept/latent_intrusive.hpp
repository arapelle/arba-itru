#pragma once

#include "sharable_intrusive.hpp"

inline namespace arba
{
namespace itru
{

template <class ElementType>
concept LatentIntrusive = SharableIntrusive<ElementType> && requires(ElementType* ptr) {
    weak_intrusive_ptr_add_ref(ptr);
    weak_intrusive_ptr_release(ptr);
    weak_intrusive_ptr_lock(ptr);
    weak_intrusive_ptr_use_count(ptr);
};

} // namespace itru
} // namespace arba
