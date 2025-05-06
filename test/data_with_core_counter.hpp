#pragma once

#include <arba/itru/intrusive_ref_counter.hpp>

#include <string>

struct base_with_ircnt : public itru::intrusive_ref_counter<>
{
    virtual ~base_with_ircnt() = default;
};

struct data_with_ircnt : public base_with_ircnt
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt(bool& bval, const std::string& input_text = "") : text(input_text), valid(&bval) { bval = true; }
    ~data_with_ircnt() { *valid = false; }
};

struct data_with_ircnt_mc : public base_with_ircnt
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt_mc(bool& bval, std::string&& input_text) : text(std::forward<std::string>(input_text)), valid(&bval)
    {
        bval = true;
    }
    ~data_with_ircnt_mc() { *valid = false; }
};

struct base_with_ircnts : public itru::intrusive_ref_counters<>
{
    virtual ~base_with_ircnts() = default;
};

struct data_with_ircnts : public base_with_ircnts
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnts(bool& bval, const std::string& input_text = "") : text(input_text), valid(&bval) { bval = true; }
    ~data_with_ircnts() { *valid = false; }
};
