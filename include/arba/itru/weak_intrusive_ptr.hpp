#pragma once

#include "shared_intrusive_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename ElementType>
class weak_intrusive_ptr
{
public:
    using element_type = ElementType;

    weak_intrusive_ptr() {}

    weak_intrusive_ptr(const weak_intrusive_ptr& wiptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    weak_intrusive_ptr(const weak_intrusive_ptr<Up>& wiptr);

    weak_intrusive_ptr(weak_intrusive_ptr&& wiptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    weak_intrusive_ptr(weak_intrusive_ptr<Up>&& wiptr);

    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    weak_intrusive_ptr(const shared_intrusive_ptr<Up>& siptr);

    ~weak_intrusive_ptr();

    weak_intrusive_ptr& operator=(weak_intrusive_ptr wiptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    weak_intrusive_ptr& operator=(weak_intrusive_ptr<Up> wiptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    weak_intrusive_ptr& operator=(const shared_intrusive_ptr<Up>& siptr);

    void reset() noexcept;

    inline std::size_t use_count() const noexcept
    {
        if (pointer_) [[likely]]
            return weak_intrusive_ptr_use_count(pointer_);
        return 0;
    }
    inline bool expired() const noexcept { return use_count() == 0; }
    inline shared_intrusive_ptr<element_type> lock() const noexcept
    {
        if (pointer_) [[likely]]
        {
            if (weak_intrusive_ptr_lock(pointer_))
                return shared_intrusive_ptr<element_type>(pointer_,
                                                          typename shared_intrusive_ptr<element_type>::lock_tag{});
            weak_intrusive_ptr_release(pointer_);
            pointer_ = nullptr;
        }
        return shared_intrusive_ptr<element_type>();
    }

    inline void swap(weak_intrusive_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const weak_intrusive_ptr&) const = default;

private:
    mutable element_type* pointer_ = nullptr;

    template <typename Up>
    friend class weak_intrusive_ptr;
};

template <typename Type>
inline weak_intrusive_ptr<Type>::weak_intrusive_ptr(const weak_intrusive_ptr& wiptr) : pointer_(wiptr.pointer_)
{
    if (pointer_)
        weak_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline weak_intrusive_ptr<Type>::weak_intrusive_ptr(const weak_intrusive_ptr<Up>& wiptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(wiptr.pointer_))
{
    if (pointer_)
        weak_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
inline weak_intrusive_ptr<Type>::weak_intrusive_ptr(weak_intrusive_ptr&& wiptr) : pointer_(wiptr.pointer_)
{
    wiptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline weak_intrusive_ptr<Type>::weak_intrusive_ptr(weak_intrusive_ptr<Up>&& wiptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(wiptr.pointer_))
{
    wiptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline weak_intrusive_ptr<Type>::weak_intrusive_ptr(const shared_intrusive_ptr<Up>& siptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(siptr.get()))
{
    if (pointer_)
        weak_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
inline weak_intrusive_ptr<Type>::~weak_intrusive_ptr()
{
    reset();
}

template <typename Type>
inline weak_intrusive_ptr<Type>& weak_intrusive_ptr<Type>::operator=(weak_intrusive_ptr<Type> wiptr)
{
    std::swap(pointer_, wiptr.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline weak_intrusive_ptr<Type>& weak_intrusive_ptr<Type>::operator=(weak_intrusive_ptr<Up> wiptr)
{
    weak_intrusive_ptr<Type> aux(std::move(wiptr));
    std::swap(pointer_, aux.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline weak_intrusive_ptr<Type>& weak_intrusive_ptr<Type>::operator=(const shared_intrusive_ptr<Up>& siptr)
{
    return *this = weak_intrusive_ptr<Type>(siptr);
}

template <typename Type>
inline void weak_intrusive_ptr<Type>::reset() noexcept
{
    if (pointer_)
    {
        weak_intrusive_ptr_release(pointer_);
        pointer_ = nullptr;
    }
}

} // namespace itru
} // namespace arba
