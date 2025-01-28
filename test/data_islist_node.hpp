#pragma once

#include <arba/itru/intrusive_ref_counter.hpp>
#include <arba/itru/intrusive_sharable_list_hook.hpp>

#include <string>

struct abstract_data_islist_node : public itru::intrusive_ref_counters<>,
                                   public itru::intrusive_sharable_list_hook<abstract_data_islist_node>
{
public:
    virtual ~abstract_data_islist_node() = default;
    virtual const std::string& text() const = 0;
};

struct sentinel_data_islist_node : public abstract_data_islist_node
{
public:
    virtual ~sentinel_data_islist_node() = default;
    virtual const std::string& text() const
    {
        assert(false);
        return *reinterpret_cast<const std::string*>(0); // !!! do not reproduce that at home.
    }
};

struct concrete_data_islist_node : public abstract_data_islist_node
{
    std::string str;
    bool* valid = nullptr;

    explicit concrete_data_islist_node(bool& valid, const std::string& text = "") : str(text), valid(&valid)
    {
        valid = true;
    }

    concrete_data_islist_node(concrete_data_islist_node&& other) : str(std::move(other.str)), valid(other.valid)
    {
        other.valid = nullptr;
    }

    ~concrete_data_islist_node()
    {
        if (valid)
        {
            *valid = false;
        }
    }

    virtual const std::string& text() const override { return str; }
};

struct data_islist_node : public itru::intrusive_ref_counters<>,
                          public itru::intrusive_sharable_list_hook<data_islist_node>
{
    std::string text;
    bool* valid = nullptr;

    data_islist_node() {}

    explicit data_islist_node(bool& valid, const std::string& text = "") : text(text), valid(&valid) { valid = true; }

    data_islist_node(data_islist_node&& other) : text(std::move(other.text)), valid(other.valid)
    {
        other.valid = nullptr;
    }

    ~data_islist_node()
    {
        if (valid)
        {
            *valid = false;
        }
    }
};
