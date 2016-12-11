//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include "policy.hpp"

namespace di
{

struct AccessPolicy {};
struct Mutable : AccessPolicy
{
    template<typename Type>
    struct Apply
    {
        template<typename T>
        void operator()(const T&) {}
    };
};

struct Immutable : AccessPolicy
{   
    template<typename Type>
    struct Apply
    {
        template<typename T>
        void operator()(const T&) { static_assert(!sizeof(T), "Cannot modify immutable object"); }
    };
};

template<>
struct StaticDefault<AccessPolicy>
{
    using type = Mutable;
};

} // namespace di

