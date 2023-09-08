#pragma once

#include "intrusive_shared_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename elem_type>
class intrusive_weak_ptr
{
public:
    using element_type = elem_type;

    intrusive_weak_ptr() {}

    intrusive_weak_ptr(const intrusive_weak_ptr& iwptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_weak_ptr(const intrusive_weak_ptr<Up>& iwptr);

    intrusive_weak_ptr(intrusive_weak_ptr&& iwptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_weak_ptr(intrusive_weak_ptr<Up>&& iwptr);

    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_weak_ptr(const intrusive_shared_ptr<Up>& isptr);

    ~intrusive_weak_ptr();

    intrusive_weak_ptr& operator=(intrusive_weak_ptr iwptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_weak_ptr& operator=(intrusive_weak_ptr<Up> iwptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_weak_ptr& operator=(const intrusive_shared_ptr<Up>& isptr);

    void reset() noexcept;

    inline std::size_t use_count() const noexcept
    {
        if (pointer_) [[likely]]
            return intrusive_weak_ptr_use_count(pointer_);
        return 0;
    }
    inline bool expired() const noexcept { return use_count() == 0; }
    inline intrusive_shared_ptr<element_type> lock() const noexcept
    {
        if (pointer_) [[likely]]
        {
            if (intrusive_weak_ptr_lock(pointer_))
                return intrusive_shared_ptr<element_type>(pointer_,
                                                          typename intrusive_shared_ptr<element_type>::lock_tag{});
            intrusive_weak_ptr_release(pointer_);
            pointer_ = nullptr;
        }
        return intrusive_shared_ptr<element_type>();
    }

    inline void swap(intrusive_weak_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const intrusive_weak_ptr&) const = default;

private:
    mutable element_type* pointer_ = nullptr;

    template <typename Up>
    friend class intrusive_weak_ptr;
};

template <typename Type>
inline intrusive_weak_ptr<Type>::intrusive_weak_ptr(const intrusive_weak_ptr& iwptr)
    : pointer_(iwptr.pointer_)
{
    if (pointer_)
        intrusive_weak_ptr_add_ref(pointer_);
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_weak_ptr<Type>::intrusive_weak_ptr(const intrusive_weak_ptr<Up>& iwptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(iwptr.pointer_))
{
    if (pointer_)
        intrusive_weak_ptr_add_ref(pointer_);
}

template <typename Type>
inline intrusive_weak_ptr<Type>::intrusive_weak_ptr(intrusive_weak_ptr&& iwptr)
    : pointer_(iwptr.pointer_)
{
    iwptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_weak_ptr<Type>::intrusive_weak_ptr(intrusive_weak_ptr<Up>&& iwptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(iwptr.pointer_))
{
    iwptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_weak_ptr<Type>::intrusive_weak_ptr(const intrusive_shared_ptr<Up>& isptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(isptr.get()))
{
    if (pointer_)
        intrusive_weak_ptr_add_ref(pointer_);
}

template <typename Type>
inline intrusive_weak_ptr<Type>::~intrusive_weak_ptr()
{
    reset();
}

template <typename Type>
inline intrusive_weak_ptr<Type>& intrusive_weak_ptr<Type>::operator=(intrusive_weak_ptr<Type> iwptr)
{
    std::swap(pointer_, iwptr.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_weak_ptr<Type>& intrusive_weak_ptr<Type>::operator=(intrusive_weak_ptr<Up> iwptr)
{
    intrusive_weak_ptr<Type> aux(std::move(iwptr));
    std::swap(pointer_, aux.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_weak_ptr<Type>& intrusive_weak_ptr<Type>::operator=(const intrusive_shared_ptr<Up>& isptr)
{
    return *this = intrusive_weak_ptr<Type>(isptr);
}

template <typename Type>
inline void intrusive_weak_ptr<Type>::reset() noexcept
{
    if (pointer_)
    {
        intrusive_weak_ptr_release(pointer_);
        pointer_ = nullptr;
    }
}

}
}

