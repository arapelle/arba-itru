#pragma once

#include "shared_intrusive_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename IntrusiveT>
class sharable_intrusive_list_iterator
{
public:
    using value_type = IntrusiveT;
    using pointer = std::add_pointer_t<IntrusiveT>;
    using reference = std::add_lvalue_reference_t<IntrusiveT>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    explicit sharable_intrusive_list_iterator(IntrusiveT& node_ref) : pointer_(&node_ref) {}

    inline sharable_intrusive_list_iterator(
        sharable_intrusive_list_iterator<std::remove_const_t<IntrusiveT>> const& iter)
        : pointer_(iter.ptr())
    {
    }

    inline sharable_intrusive_list_iterator& operator=(sharable_intrusive_list_iterator const& iter) = default;

    sharable_intrusive_list_iterator& operator++() noexcept
    {
        pointer_ = pointer_->next().get();
        return *this;
    }
    sharable_intrusive_list_iterator operator++(int) noexcept
    {
        sharable_intrusive_list_iterator iter(*this);
        ++(*this);
        return iter;
    }
    sharable_intrusive_list_iterator& operator--() noexcept
    {
        pointer_ = pointer_->previous();
        return *this;
    }
    sharable_intrusive_list_iterator operator--(int) noexcept
    {
        sharable_intrusive_list_iterator iter(*this);
        --(*this);
        return iter;
    }

    const reference operator*() const noexcept { return *pointer_; }
    reference operator*() noexcept { return *pointer_; }
    const pointer operator->() const noexcept { return pointer_; }
    pointer operator->() noexcept { return pointer_; }

    inline pointer ptr() const noexcept { return pointer_; }
    inline shared_intrusive_ptr<value_type> make_intrusive_shared() const noexcept
    {
        return shared_intrusive_ptr<value_type>(pointer_);
    }

    auto operator<=>(const sharable_intrusive_list_iterator&) const noexcept = default;

private:
    pointer pointer_;
};

template <typename IntrusiveT, typename SentinelT = IntrusiveT>
class sharable_intrusive_list
{
public:
    using value_type = IntrusiveT;
    using size_type = std::size_t;
    using reference = std::add_lvalue_reference_t<value_type>;
    using const_reference = std::add_lvalue_reference_t<std::add_const_t<value_type>>;
    using pointer = std::add_pointer_t<value_type>;
    using const_pointer = std::add_pointer_t<std::add_const_t<value_type>>;
    using iterator = sharable_intrusive_list_iterator<value_type>;
    using const_iterator = sharable_intrusive_list_iterator<const value_type>;
    using difference_type = std::ptrdiff_t;
    using value_siptr_type = shared_intrusive_ptr<value_type>;

public:
    sharable_intrusive_list();
    explicit sharable_intrusive_list(SentinelT sentinel);

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

    void push_front(value_siptr_type value_siptr);
    void push_back(value_siptr_type value_siptr);
    const_iterator insert(const_iterator iter, value_siptr_type value_siptr);

    template <class... ArgsT>
    inline void emplace_front(ArgsT&&... args)
    {
        push_front(make_shared_intrusive_ptr<value_type>(std::forward<ArgsT>(args)...));
    }
    template <class... ArgsT>
    inline void emplace_back(ArgsT&&... args)
    {
        push_back(make_shared_intrusive_ptr<value_type>(std::forward<ArgsT>(args)...));
    }
    template <class... ArgsT>
    inline const_iterator emplace(const_iterator iter, ArgsT&&... args)
    {
        return insert(iter, make_shared_intrusive_ptr<value_type>(std::forward<ArgsT>(args)...));
    }

    void pop_front();
    void pop_back();
    iterator erase(iterator iter);

    void clear();
    void swap(sharable_intrusive_list& other);

    inline void splice(iterator iter, sharable_intrusive_list& other);
    inline void splice(iterator iter, sharable_intrusive_list&& other);
    inline void splice(iterator iter, sharable_intrusive_list& other, iterator it);
    inline void splice(iterator iter, sharable_intrusive_list& other, iterator first, iterator last);

    template <class UnaryPredicate>
    size_type remove_if(UnaryPredicate predicate);

private:
    void splice_(iterator iter, sharable_intrusive_list& other, iterator first, iterator last, std::size_t size);

    static void hook_after_(value_type& list_value_ref, value_siptr_type&& value_siptr);
    static void unhook_(value_type& list_value_ref);

private:
    SentinelT sentinel_;
    size_type size_ = 0;
};

template <class IntrusiveT, typename SentinelT>
sharable_intrusive_list<IntrusiveT, SentinelT>::sharable_intrusive_list()
{
    shared_intrusive_ptr_add_ref(&sentinel_);
    sentinel_.previous() = &sentinel_;
    sentinel_.next() = value_siptr_type(&sentinel_);
}

template <class IntrusiveT, typename SentinelT>
sharable_intrusive_list<IntrusiveT, SentinelT>::sharable_intrusive_list(SentinelT sentinel)
    : sentinel_(std::move(sentinel))
{
    shared_intrusive_ptr_add_ref(&sentinel_);
    sentinel_.previous() = &sentinel_;
    sentinel_.next() = value_siptr_type(&sentinel_);
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::push_front(value_siptr_type value_siptr)
{
    hook_after_(sentinel_, std::move(value_siptr));
    ++size_;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::push_back(value_siptr_type value_siptr)
{
    hook_after_(*sentinel_.previous(), std::move(value_siptr));
    ++size_;
}

template <class IntrusiveT, typename SentinelT>
typename sharable_intrusive_list<IntrusiveT, SentinelT>::const_iterator
sharable_intrusive_list<IntrusiveT, SentinelT>::insert(const_iterator iter, value_siptr_type value_siptr)
{
    const_iterator res_iter(*value_siptr);
    hook_after_(*iter->previous(), std::move(value_siptr));
    ++size_;
    return res_iter;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::pop_front()
{
    unhook_(*sentinel_.next());
    --size_;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::pop_back()
{
    unhook_(*sentinel_.previous());
    --size_;
}

template <class IntrusiveT, typename SentinelT>
typename sharable_intrusive_list<IntrusiveT, SentinelT>::iterator
sharable_intrusive_list<IntrusiveT, SentinelT>::erase(iterator iter)
{
    iterator res_iter(*iter->next());
    unhook_(*iter);
    --size_;
    return res_iter;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::clear()
{
    value_siptr_type& sentinel_next = sentinel_.next();
    for (value_type* first_value = sentinel_next.get(); first_value != &sentinel_; first_value = sentinel_next.get())
    {
        first_value->previous() = nullptr;
        sentinel_next = std::move(first_value->next());
    }
    sentinel_.previous() = &sentinel_;
    size_ = 0;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::swap(sharable_intrusive_list& other)
{
    sentinel_.next().swap(other.sentinel_.next());
    std::swap(sentinel_.previous(), other.sentinel_.previous());
    std::swap(size_, other.size_);
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::splice(iterator iter, sharable_intrusive_list& other)
{
    splice_(iter, other, other.begin(), other.end(), other.size());
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::splice(iterator iter, sharable_intrusive_list&& other)
{
    splice_(iter, other, other.begin(), other.end(), other.size());
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::splice(iterator iter, sharable_intrusive_list& other, iterator it)
{
    value_siptr_type aux_siptr = it.make_intrusive_shared();
    unhook_(*it);
    hook_after_(*iter->previous(), std::move(aux_siptr));
    ++size_;
    --other.size_;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::splice(iterator iter, sharable_intrusive_list& other,
                                                            iterator first, iterator last)
{
    splice_(iter, other, first, last, std::distance(first, last));
}

template <class IntrusiveT, typename SentinelT>
template <class UnaryPredicate>
typename sharable_intrusive_list<IntrusiveT, SentinelT>::size_type
sharable_intrusive_list<IntrusiveT, SentinelT>::remove_if(UnaryPredicate predicate)
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
void sharable_intrusive_list<IntrusiveT, SentinelT>::splice_(iterator iter, sharable_intrusive_list& other,
                                                             iterator first, iterator last, std::size_t size)
{
    pointer iter_previous = iter->previous();
    pointer first_previous = first->previous();
    pointer last_previous = last->previous();
    iter->previous() = last_previous;
    last->previous() = first_previous;
    first->previous() = last_previous;
    value_siptr_type iter_siptr = std::move(iter_previous->next());
    iter_previous->next() = std::move(first_previous->next());
    first_previous->next() = std::move(last_previous->next());
    last_previous->next() = std::move(iter_siptr);
    size_ += size;
    other.size_ -= size;
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::hook_after_(value_type& list_value_ref,
                                                                 value_siptr_type&& value_siptr)
{
    value_siptr->previous() = &list_value_ref;
    list_value_ref.next()->previous() = value_siptr.get();
    value_siptr->next() = std::move(list_value_ref.next());
    list_value_ref.next() = std::move(value_siptr);
}

template <class IntrusiveT, typename SentinelT>
void sharable_intrusive_list<IntrusiveT, SentinelT>::unhook_(value_type& vref)
{
    IntrusiveT* previous = vref.previous();
    vref.next()->previous() = previous;
    vref.previous() = nullptr;
    previous->next() = std::move(vref.next());
}

} // namespace itru
} // namespace arba

template <class ValueT>
inline void std::swap(::arba::itru::sharable_intrusive_list<ValueT>& lhs,
                      ::arba::itru::sharable_intrusive_list<ValueT>& rhs) noexcept
{
    lhs.swap(rhs);
}

template <class ValueT, class UnaryPredicate>
inline typename ::arba::itru::sharable_intrusive_list<ValueT>::size_type
std::erase_if(::arba::itru::sharable_intrusive_list<ValueT>& lhs, UnaryPredicate predicate)
{
    return lhs.remove_if(predicate);
}
