#pragma once

//local
#include "policy.hpp"

//std
#include <utility>

namespace di
{

struct MixinPolicy {};

template<typename Subject>
struct Mixin : MixinPolicy
{
    template<typename Type>
    struct Apply : public Subject
    {
        Apply() = default;

        template<typename U>
        explicit Apply(U&& param) : Subject(std::forward<U>(param)) {}
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

