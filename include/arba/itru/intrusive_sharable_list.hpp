#pragma once

#include "intrusive_shared_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename IntrusiveT>
class intrusive_sharable_list_iterator
{
public:
    using value_type = IntrusiveT;
    using pointer = std::add_pointer_t<IntrusiveT>;
    using reference = std::add_lvalue_reference_t<IntrusiveT>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    explicit intrusive_sharable_list_iterator(IntrusiveT& node_ref) : pointer_(&node_ref) {}

    inline intrusive_sharable_list_iterator(
        intrusive_sharable_list_iterator<std::remove_const_t<IntrusiveT>> const& iter)
        : pointer_(iter.ptr())
    {
    }

    inline intrusive_sharable_list_iterator& operator=(intrusive_sharable_list_iterator const& iter) = default;

    intrusive_sharable_list_iterator& operator++() noexcept
    {
        pointer_ = pointer_->next().get();
        return *this;
    }
    intrusive_sharable_list_iterator operator++(int) noexcept
    {
        intrusive_sharable_list_iterator iter(*this);
        ++(*this);
        return iter;
    }
    intrusive_sharable_list_iterator& operator--() noexcept
    {
        pointer_ = pointer_->previous();
        return *this;
    }
    intrusive_sharable_list_iterator operator--(int) noexcept
    {
        intrusive_sharable_list_iterator iter(*this);
        --(*this);
        return iter;
    }

    const reference operator*() const noexcept { return *pointer_; }
    reference operator*() noexcept { return *pointer_; }
    const pointer operator->() const noexcept { return pointer_; }
    pointer operator->() noexcept { return pointer_; }

    inline pointer ptr() const noexcept { return pointer_; }
    inline intrusive_shared_ptr<value_type> make_intrusive_shared() const noexcept
    {
        return intrusive_shared_ptr<value_type>(pointer_);
    }

    auto operator<=>(const intrusive_sharable_list_iterator&) const noexcept = default;

private:
    pointer pointer_;
};

template <typename IntrusiveT, typename SentinelT = IntrusiveT>
class intrusive_sharable_list
{
public:
    using value_type = IntrusiveT;
    using size_type = std::size_t;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
    using iterator = intrusive_sharable_list_iterator<value_type>;
    using const_iterator = intrusive_sharable_list_iterator<const value_type>;
    using difference_type = std::ptrdiff_t;
    using value_isptr_type = intrusive_shared_ptr<value_type>;

public:
    intrusive_sharable_list();
    explicit intrusive_sharable_list(SentinelT sentinel);

    inline const_iterator begin() const noexcept { return const_iterator(*sentinel_.next()); }
    inline iterator begin() noexcept { return iterator(*sentinel_.next()); }
    inline const_iterator cbegin() const noexcept { return begin(); }

    inline const_iterator end() const noexcept { return const_iterator(sentinel_); }
    inline iterator end() noexcept { return iterator(sentinel_); }
    inline const_iterator cend() const noexcept { return end(); }

    inline bool empty() const noexcept { return size_ == 0; }
    inline std::size_t size() const noexcept { return size_; }

    inline const_reference front() const noexcept { return *sentinel_.next(); }
    inline reference front() noexcept { return *sentinel_.next(); }

    inline const_reference back() const noexcept { return *sentinel_.previous(); }
    inline reference back() noexcept { return *sentinel_.previous(); }

    void push_front(value_isptr_type value_isptr);
    void push_back(value_isptr_type value_isptr);
    const_iterator insert(const_iterator iter, value_isptr_type value_isptr);

    template <class... ArgsT>
    inline void emplace_front(ArgsT&&... args)
    {
        push_front(make_intrusive_shared_ptr<value_type>(std::forward<ArgsT>(args)...));
    }
    template <class... ArgsT>
    inline void emplace_back(ArgsT&&... args)
    {
        push_back(make_intrusive_shared_ptr<value_type>(std::forward<ArgsT>(args)...));
    }
    template <class... ArgsT>
    inline const_iterator emplace(const_iterator iter, ArgsT&&... args)
    {
        return insert(iter, make_intrusive_shared_ptr<value_type>(std::forward<ArgsT>(args)...));
    }

    void pop_front();
    void pop_back();
    iterator erase(iterator iter);

    void clear();
    void swap(intrusive_sharable_list& other);

    inline void splice(iterator iter, intrusive_sharable_list& other);
    inline void splice(iterator iter, intrusive_sharable_list&& other);
    inline void splice(iterator iter, intrusive_sharable_list& other, iterator it);
    inline void splice(iterator iter, intrusive_sharable_list& other, iterator first, iterator last);

    template <class UnaryPredicate>
    size_type remove_if(UnaryPredicate predicate);

private:
    void splice_(iterator iter, intrusive_sharable_list& other, iterator first, iterator last, std::size_t size);

    static void hook_after_(value_type& list_value_ref, value_isptr_type&& value_isptr);
    static void unhook_(value_type& list_value_ref);

private:
    SentinelT sentinel_;
    size_type size_ = 0;
};

template <class IntrusiveT, typename SentinelT>
intrusive_sharable_list<IntrusiveT, SentinelT>::intrusive_sharable_list()
{
    intrusive_shared_ptr_add_ref(&sentinel_);
    sentinel_.previous() = &sentinel_;
    sentinel_.next() = value_isptr_type(&sentinel_);
}

template <class IntrusiveT, typename SentinelT>
intrusive_sharable_list<IntrusiveT, SentinelT>::intrusive_sharable_list(SentinelT sentinel)
    : sentinel_(std::move(sentinel))
{
    intrusive_shared_ptr_add_ref(&sentinel_);
    sentinel_.previous() = &sentinel_;
    sentinel_.next() = value_isptr_type(&sentinel_);
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::push_front(value_isptr_type value_isptr)
{
    hook_after_(sentinel_, std::move(value_isptr));
    ++size_;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::push_back(value_isptr_type value_isptr)
{
    hook_after_(*sentinel_.previous(), std::move(value_isptr));
    ++size_;
}

template <class IntrusiveT, typename SentinelT>
typename intrusive_sharable_list<IntrusiveT, SentinelT>::const_iterator
intrusive_sharable_list<IntrusiveT, SentinelT>::insert(const_iterator iter, value_isptr_type value_isptr)
{
    const_iterator res_iter(*value_isptr);
    hook_after_(*iter->previous(), std::move(value_isptr));
    ++size_;
    return res_iter;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::pop_front()
{
    unhook_(*sentinel_.next());
    --size_;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::pop_back()
{
    unhook_(*sentinel_.previous());
    --size_;
}

template <class IntrusiveT, typename SentinelT>
typename intrusive_sharable_list<IntrusiveT, SentinelT>::iterator
intrusive_sharable_list<IntrusiveT, SentinelT>::erase(iterator iter)
{
    iterator res_iter(*iter->next());
    unhook_(*iter);
    --size_;
    return res_iter;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::clear()
{
    value_isptr_type& sentinel_next = sentinel_.next();
    for (value_type* first_value = sentinel_next.get(); first_value != &sentinel_; first_value = sentinel_next.get())
    {
        first_value->previous() = nullptr;
        sentinel_next = std::move(first_value->next());
    }
    sentinel_.previous() = &sentinel_;
    size_ = 0;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::swap(intrusive_sharable_list& other)
{
    sentinel_.next().swap(other.sentinel_.next());
    std::swap(sentinel_.previous(), other.sentinel_.previous());
    std::swap(size_, other.size_);
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::splice(iterator iter, intrusive_sharable_list& other)
{
    splice_(iter, other, other.begin(), other.end(), other.size());
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::splice(iterator iter, intrusive_sharable_list&& other)
{
    splice_(iter, other, other.begin(), other.end(), other.size());
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::splice(iterator iter, intrusive_sharable_list& other, iterator it)
{
    value_isptr_type aux_isptr = it.make_intrusive_shared();
    unhook_(*it);
    hook_after_(*iter->previous(), std::move(aux_isptr));
    ++size_;
    --other.size_;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::splice(iterator iter, intrusive_sharable_list& other,
                                                            iterator first, iterator last)
{
    splice_(iter, other, first, last, std::distance(first, last));
}

template <class IntrusiveT, typename SentinelT>
template <class UnaryPredicate>
typename intrusive_sharable_list<IntrusiveT, SentinelT>::size_type
intrusive_sharable_list<IntrusiveT, SentinelT>::remove_if(UnaryPredicate predicate)
{
    size_type count = 0;
    for (auto iter = begin(), end_iter = end(); iter != end_iter;)
    {
        if (predicate(*iter))
        {
            iter = erase(iter);
            ++count;
        }
        else
            ++iter;
    }
    return count;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::splice_(iterator iter, intrusive_sharable_list& other,
                                                             iterator first, iterator last, std::size_t size)
{
    pointer iter_previous = iter->previous();
    pointer first_previous = first->previous();
    pointer last_previous = last->previous();
    iter->previous() = last_previous;
    last->previous() = first_previous;
    first->previous() = last_previous;
    value_isptr_type iter_isptr = std::move(iter_previous->next());
    iter_previous->next() = std::move(first_previous->next());
    first_previous->next() = std::move(last_previous->next());
    last_previous->next() = std::move(iter_isptr);
    size_ += size;
    other.size_ -= size;
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::hook_after_(value_type& list_value_ref,
                                                                 value_isptr_type&& value_isptr)
{
    value_isptr->previous() = &list_value_ref;
    list_value_ref.next()->previous() = value_isptr.get();
    value_isptr->next() = std::move(list_value_ref.next());
    list_value_ref.next() = std::move(value_isptr);
}

template <class IntrusiveT, typename SentinelT>
void intrusive_sharable_list<IntrusiveT, SentinelT>::unhook_(value_type& vref)
{
    IntrusiveT* previous = vref.previous();
    vref.next()->previous() = previous;
    vref.previous() = nullptr;
    previous->next() = std::move(vref.next());
}

} // namespace itru
} // namespace arba

template <class ValueT>
inline void std::swap(::arba::itru::intrusive_sharable_list<ValueT>& lhs,
                      ::arba::itru::intrusive_sharable_list<ValueT>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class ValueT, class UnaryPredicate>
inline typename ::arba::itru::intrusive_sharable_list<ValueT>::size_type
std::erase_if(::arba::itru::intrusive_sharable_list<ValueT>& lhs, UnaryPredicate predicate)
{
    return lhs.remove_if(predicate);
}
