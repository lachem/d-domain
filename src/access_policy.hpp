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

