#pragma once

#include "intrusive_shared_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename IntrusiveListNodeT>
class intrusive_sharable_list_hook
{
public:
    inline IntrusiveListNodeT* previous() const noexcept { return previous_; }
    inline IntrusiveListNodeT*& previous() noexcept { return previous_; }
    inline const intrusive_shared_ptr<IntrusiveListNodeT>& next() const noexcept { return next_; }
    inline intrusive_shared_ptr<IntrusiveListNodeT>& next() noexcept { return next_; }
    void unhook()
    {
        next_->previous() = previous_;
        IntrusiveListNodeT* previous = previous_;
        previous_ = nullptr;
        previous->next_ = std::move(std::move(next_));
    }

private:
    IntrusiveListNodeT* previous_ = nullptr;
    intrusive_shared_ptr<IntrusiveListNodeT> next_;
};

} // namespace itru
} // namespace arba
