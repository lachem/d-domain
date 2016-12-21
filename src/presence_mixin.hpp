//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace di {

struct Required
{
    Required() = delete;

    template<typename T>
    explicit Required(T&&) {}

    template<typename T>
    void set(const T&) {}

    bool empty() const { return false; }
    bool initialized() const { return true; }
};

struct Optional
{
    Optional() = default;

    template<typename T>
    explicit Optional(T&&) : init(true) {}

    template<typename T>
    void set(const T&) { init = true; }

    bool empty() const { return !init; }
    bool initialized() const { return init; }

private:
    bool init = false;
};

} // namespace di

