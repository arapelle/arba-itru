#pragma once

#include <string>
#include <arba/itru/intrusive_ref_counter.hpp>

struct base_with_ircnt : public itru::intrusive_ref_counter<>
{
    virtual ~base_with_ircnt() = default;
};

struct data_with_ircnt : public base_with_ircnt
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt(bool& valid, const std::string& text = "") : text(text), valid(&valid) { valid = true; }
    ~data_with_ircnt() { *valid = false; }
};

struct data_with_ircnt_mc : public base_with_ircnt
{
    std::string text;
    bool* valid = nullptr;

    explicit data_with_ircnt_mc(bool& valid, std::string&& text) : text(std::forward<std::string>(text)), valid(&valid) { valid = true; }
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

    explicit data_with_ircnts(bool& valid, const std::string& text = "") : text(text), valid(&valid) { valid = true; }
    ~data_with_ircnts() { *valid = false; }
};
