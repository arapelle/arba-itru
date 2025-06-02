#pragma once

#include <cassert>
#include <functional>

inline namespace arba
{
namespace itru
{

template <typename ElementType>
class shared_intrusive_ptr
{
public:
    using element_type = ElementType;
    using pointer_type = std::add_pointer_t<element_type>;
    using reference_type = std::add_lvalue_reference_t<element_type>;

    shared_intrusive_ptr(std::nullptr_t = nullptr) {}

    explicit shared_intrusive_ptr(element_type* ptr);

    shared_intrusive_ptr(const shared_intrusive_ptr& siptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    shared_intrusive_ptr(const shared_intrusive_ptr<Up>& siptr);

    shared_intrusive_ptr(shared_intrusive_ptr&& siptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    shared_intrusive_ptr(shared_intrusive_ptr<Up>&& siptr);

    ~shared_intrusive_ptr();

    shared_intrusive_ptr& operator=(shared_intrusive_ptr siptr);
    template <typename Up>
        requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<element_type>>
    shared_intrusive_ptr& operator=(shared_intrusive_ptr<Up> siptr);

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

    inline void swap(shared_intrusive_ptr& other) { std::swap(pointer_, other.pointer_); }
    inline auto operator<=>(const shared_intrusive_ptr&) const = default;

    struct lock_tag
    {
    };

private:
    shared_intrusive_ptr(element_type* ptr, lock_tag);

    element_type* pointer_ = nullptr;

    template <typename Up>
    friend class shared_intrusive_ptr;
    template <typename Up>
    friend class weak_intrusive_ptr;
};

template <typename Type>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(Type* ptr) : pointer_(ptr)
{
    if (pointer_)
        shared_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(element_type* ptr, lock_tag) : pointer_(ptr)
{
}

template <typename Type>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(const shared_intrusive_ptr& siptr) : pointer_(siptr.pointer_)
{
    if (pointer_)
        shared_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(const shared_intrusive_ptr<Up>& siptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(siptr.get()))
{
    if (pointer_)
        shared_intrusive_ptr_add_ref(pointer_);
}

template <typename Type>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(shared_intrusive_ptr&& siptr) : pointer_(siptr.pointer_)
{
    siptr.pointer_ = nullptr;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline shared_intrusive_ptr<Type>::shared_intrusive_ptr(shared_intrusive_ptr<Up>&& siptr)
    : pointer_(static_cast<std::add_pointer_t<Type>>(siptr.get()))
{
    siptr.pointer_ = nullptr;
}

template <typename Type>
inline shared_intrusive_ptr<Type>::~shared_intrusive_ptr()
{
    if (pointer_)
        shared_intrusive_ptr_release(pointer_);
}

template <typename Type>
inline shared_intrusive_ptr<Type>& shared_intrusive_ptr<Type>::operator=(shared_intrusive_ptr<Type> siptr)
{
    std::swap(pointer_, siptr.pointer_);
    return *this;
}

template <typename Type>
template <typename Up>
    requires std::is_convertible_v<std::add_pointer_t<Up>, std::add_pointer_t<Type>>
inline shared_intrusive_ptr<Type>& shared_intrusive_ptr<Type>::operator=(shared_intrusive_ptr<Up> siptr)
{
    shared_intrusive_ptr<Type> aux(std::move(siptr));
    std::swap(pointer_, aux.pointer_);
    return *this;
}

template <typename Type>
inline Type* shared_intrusive_ptr<Type>::release() noexcept
{
    if (pointer_)
    {
        Type* ptr = pointer_;
        shared_intrusive_ptr_release(pointer_);
        pointer_ = nullptr;
        return ptr;
    }
    return nullptr;
}

template <typename Type>
inline void shared_intrusive_ptr<Type>::reset(Type* pointer) noexcept
{
    if (pointer_)
        shared_intrusive_ptr_release(pointer_);
    if (pointer_ = pointer; pointer_)
        shared_intrusive_ptr_add_ref(pointer_);
}

template <typename val_type, class... args_types>
inline shared_intrusive_ptr<val_type> make_shared_intrusive_ptr(args_types&&... args)
{
    return shared_intrusive_ptr<val_type>(new val_type(std::forward<args_types>(args)...));
}

} // namespace itru
} // namespace arba

template <class value_type>
struct std::hash<::arba::itru::shared_intrusive_ptr<value_type>> : private std::hash<value_type*>
{
    std::size_t operator()(const ::arba::itru::shared_intrusive_ptr<value_type>& ptr) const noexcept
    {
        return this->std::hash<value_type*>::operator()(ptr.get());
    }
};
