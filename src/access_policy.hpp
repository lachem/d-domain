//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace di {

template<typename Type>
struct Mutable
{
    Mutable() = default;

    template<typename T>
    Mutable(T&&) {}

    template<typename T>
    void set(const T&) {}
};

template<typename Type>
struct Immutable
{
    Immutable() = default;

    template<typename T>
    Immutable(T&&) {}

    template<typename T>
    void set(const T&) { static_assert(!sizeof(T), "Cannot modify immutable object"); }
};

} // namespace di

