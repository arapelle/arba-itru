#pragma once

#include "shared_intrusive_ptr.hpp"

inline namespace arba
{
namespace itru
{

template <typename IntrusiveListNodeT>
class sharable_intrusive_list_hook
{
public:
    inline IntrusiveListNodeT* previous() const noexcept { return previous_; }
    inline IntrusiveListNodeT*& previous() noexcept { return previous_; }
    inline const shared_intrusive_ptr<IntrusiveListNodeT>& next() const noexcept { return next_; }
    inline shared_intrusive_ptr<IntrusiveListNodeT>& next() noexcept { return next_; }
    void unhook()
    {
        next_->previous() = previous_;
        IntrusiveListNodeT* previous = previous_;
        previous_ = nullptr;
        previous->next_ = std::move(std::move(next_));
    }

private:
    IntrusiveListNodeT* previous_ = nullptr;
    shared_intrusive_ptr<IntrusiveListNodeT> next_;
};

} // namespace itru
} // namespace arba
