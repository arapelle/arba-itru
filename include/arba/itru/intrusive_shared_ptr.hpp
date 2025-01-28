#pragma once

#include <cassert>
#include <functional>

inline namespace arba
{
namespace itru
{

template <typename elem_type>
class intrusive_shared_ptr
{
public:
    using element_type = elem_type;
    using pointer_type = std::add_pointer_t<element_type>;
    using reference_type = std::add_lvalue_reference_t<element_type>;

    intrusive_shared_ptr(std::nullptr_t = nullptr) {}

    explicit intrusive_shared_ptr(element_type* ptr);

    intrusive_shared_ptr(const intrusive_shared_ptr& isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr(const intrusive_shared_ptr<Up>& isptr);

    intrusive_shared_ptr(intrusive_shared_ptr&& isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr(intrusive_shared_ptr<Up>&& isptr);

    ~intrusive_shared_ptr();

    intrusive_shared_ptr& operator=(intrusive_shared_ptr isptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    intrusive_shared_ptr& operator=(intrusive_shared_ptr<Up> isptr);

    element_type* release() noexcept;
    void reset(element_type* pointer = nullptr) noexcept;

    inline element_type* get() const noexcept { return pointer_; }
    inline element_type& operator*() const noexcept
    {
        assert(pointer_);
        return *pointer_;
    }
    inline element_type* operator->() const noexcept
    {
        assert(pointer_);
        return pointer_;
    }
    inline operator bool() const { return pointer_ != nullptr; }

    inline void swap(intrusive_shared_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const intrusive_shared_ptr&) const = default;

    struct lock_tag
    {
    };

private:
    intrusive_shared_ptr(element_type* ptr, lock_tag);

    element_type* pointer_ = nullptr;

    template <typename Up>
    friend class intrusive_shared_ptr;
    template <typename Up>
    friend class intrusive_weak_ptr;
};

template <typename Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(Type* ptr) : pointer_(ptr)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <typename Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(element_type* ptr, lock_tag) : pointer_(ptr)
{
}

template <typename Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(const intrusive_shared_ptr& isptr) : pointer_(isptr.pointer_)
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(const intrusive_shared_ptr<Up>& isptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(isptr.get()))
{
    if (pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <typename Type>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(intrusive_shared_ptr&& isptr) : pointer_(isptr.pointer_)
{
    isptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>::intrusive_shared_ptr(intrusive_shared_ptr<Up>&& isptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(isptr.get()))
{
    isptr.pointer_ = nullptr;
}

template <typename Type>
inline intrusive_shared_ptr<Type>::~intrusive_shared_ptr()
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
}

template <typename Type>
inline intrusive_shared_ptr<Type>& intrusive_shared_ptr<Type>::operator=(intrusive_shared_ptr<Type> isptr)
{
    std::swap(pointer_, isptr.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline intrusive_shared_ptr<Type>& intrusive_shared_ptr<Type>::operator=(intrusive_shared_ptr<Up> isptr)
{
    intrusive_shared_ptr<Type> aux(std::move(isptr));
    std::swap(pointer_, aux.pointer_);
    return *this;
}

template <typename Type>
inline Type* intrusive_shared_ptr<Type>::release() noexcept
{
    if (pointer_)
    {
        Type* ptr = pointer_;
        intrusive_shared_ptr_release(pointer_);
        pointer_ = nullptr;
        return ptr;
    }
    return nullptr;
}

template <typename Type>
inline void intrusive_shared_ptr<Type>::reset(Type* pointer) noexcept
{
    if (pointer_)
        intrusive_shared_ptr_release(pointer_);
    if (pointer_ = pointer; pointer_)
        intrusive_shared_ptr_add_ref(pointer_);
}

template <typename val_type, class... args_types>
inline intrusive_shared_ptr<val_type> make_intrusive_shared_ptr(args_types&&... args)
{
    return intrusive_shared_ptr<val_type>(new val_type(std::forward<args_types>(args)...));
}

} // namespace itru
} // namespace arba

template <class value_type>
struct std::hash<::arba::itru::intrusive_shared_ptr<value_type>> : private std::hash<value_type*>
{
    std::size_t operator()(const ::arba::itru::intrusive_shared_ptr<value_type>& ptr) const noexcept
    {
        return this->std::hash<value_type*>::operator()(ptr.get());
    }
};
