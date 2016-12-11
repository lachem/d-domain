//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include "policy.hpp"

//std
#include <utility>
#include <type_traits>

namespace di
{

struct MixinPolicy {};

template<typename... Subject>
struct Mixin : MixinPolicy
{
    template<typename Type>
    struct Apply : public Subject...
    {
        template<typename T>
        using RemoveQualifiers = typename std::decay<T>::type;

        Apply() = default;

        template<typename... T>
        explicit Apply(T&&... param) 
            : RemoveQualifiers<T>(std::forward<T>(param))... 
        {}
    };
};

struct NullMixin : MixinPolicy
{
    template<typename Type>
    struct Apply
    {
    };
};

template<>
struct StaticDefault<MixinPolicy>
{
    using type = NullMixin;
};

} // namespace di

