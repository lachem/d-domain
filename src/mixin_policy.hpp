//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//std
#include <utility>
#include <type_traits>

namespace di
{

template<template <typename> class T, typename Base>
struct Mixin : public T<Base>
{
private:
    template<typename U>
    using catch_constructible = typename std::enable_if<std::is_constructible<T<Base>, U>::value>::type*;

    template<typename U>
    using catch_non_constructible = typename std::enable_if<!std::is_constructible<T<Base>, U>::value>::type*;

public:
    Mixin() = default;

    template<typename U>
    explicit Mixin(U&& value, catch_constructible<U> = nullptr)
        : T<Base>(std::forward<U>(value))
    {}

    template<typename U>
    explicit Mixin(U&&, catch_non_constructible<U> = nullptr)
    {}
};

template<template <typename> class... Subject>
struct MixinPolicy
{
    template<typename Type>
    struct Apply : public Mixin<Subject, Apply<Type>>...
    {
        Apply() = default;

        template<typename T>
        explicit Apply(const T& value)
            : Mixin<Subject, Apply<Type>>(value)...
        {}

        template<typename T>
        static Type* self(T* object) { return static_cast<Type*>(object); }

        template<typename T>
        static const Type* self(const T* object) { return static_cast<const Type*>(object); }
    };
};

} // namespace di

