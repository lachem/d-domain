//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//std
#include <utility>
#include <type_traits>

namespace di {
namespace detail {

template <typename C, typename F, typename = void>
struct is_call_possible : public std::false_type {};

template <typename C, typename R, typename... A>
struct is_call_possible<C, R(A...),
    typename std::enable_if<
        std::is_same<R, void>::value ||
        std::is_convertible<decltype(
            std::declval<C>().set(std::declval<A>()...)
        ), R>::value
    >::type
> : public std::true_type {};

} // namespace detail

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

    template<typename U>
    void set(const U& value)
    {
        doSet(value, detail::is_call_possible<T<Base>, void(U)>());
    }

private:
    template<typename U>
    void doSet(const U& value, std::true_type)
    {
        T<Base>::set(value);
    }

    template<typename U>
    void doSet(const U&, std::false_type)
    {
    }
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
        void set(const T& value)
        {
            doSet<T, Mixin<Subject, Apply<Type>>...>(value);
        }

        template<typename T>
        static Type* self(T* object) { return static_cast<Type*>(object); }

        template<typename T>
        static const Type* self(const T* object) { return static_cast<const Type*>(object); }

    private:
        template<typename T, typename U1, typename U2, typename... U>
        void doSet(const T& value)
        {
            static_cast<U1*>(this)->set(value);
            doSet<T,U2,U...>(value);
        }

        template<typename T, typename U1>
        void doSet(const T& value)
        {
            static_cast<U1*>(this)->set(value);
        }
    };
};

} // namespace di

