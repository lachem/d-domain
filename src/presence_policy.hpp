//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace di
{

template<typename Type>
struct Required
{
    Required() = delete;

    template<typename T>
    explicit Required(T&&) {}

    template<typename T>
    void set(const T&) {}

    bool initialized() const { return true; }
};

template<typename Type>
struct Optional
{
    Optional() = default;

    template<typename T>
    explicit Optional(T&&) : init(true) {}

    template<typename T>
    void set(const T&) { init = true; }

    bool initialized() const { return static_cast<bool>(init); }

private:
    bool init = false;
};

} // namespace di

