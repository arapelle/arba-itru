#pragma once

#include <arba/itru/intrusive_ref_counter.hpp>
#include <arba/itru/sharable_intrusive_list_hook.hpp>

#include <string>

struct abstract_data_silist_node : public itru::intrusive_ref_counters<>,
                                   public itru::sharable_intrusive_list_hook<abstract_data_silist_node>
{
public:
    virtual ~abstract_data_silist_node() = default;
    virtual const std::string& text() const = 0;
};

struct sentinel_data_silist_node : public abstract_data_silist_node
{
public:
    virtual ~sentinel_data_silist_node() = default;
    virtual const std::string& text() const
    {
        assert(false);
        return *reinterpret_cast<const std::string*>(0); // !!! do not reproduce that at home.
    }
};

struct concrete_data_silist_node : public abstract_data_silist_node
{
    std::string str;
    bool* valid = nullptr;

    explicit concrete_data_silist_node(bool& bval, const std::string& text = "") : str(text), valid(&bval)
    {
        bval = true;
    }

    concrete_data_silist_node(concrete_data_silist_node&& other) : str(std::move(other.str)), valid(other.valid)
    {
        other.valid = nullptr;
    }

    ~concrete_data_silist_node()
    {
        if (valid)
        {
            *valid = false;
        }
    }

    virtual const std::string& text() const override { return str; }
};

struct data_silist_node : public itru::intrusive_ref_counters<>,
                          public itru::sharable_intrusive_list_hook<data_silist_node>
{
    std::string text;
    bool* valid = nullptr;

    data_silist_node() {}

    explicit data_silist_node(bool& bval, const std::string& input_text = "") : text(input_text), valid(&bval) { bval = true; }

    data_silist_node(data_silist_node&& other) : text(std::move(other.text)), valid(other.valid)
    {
        other.valid = nullptr;
    }

    ~data_silist_node()
    {
        if (valid)
        {
            *valid = false;
        }
    }
};
