//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include "traits.hpp"

//std
#include <utility>
#include <type_traits>

namespace di {
namespace detail {

template <typename C, typename F, typename = void>
struct is_set_available : public std::false_type {};

template <typename C, typename R, typename... A>
struct is_set_available<C, R(A...),
    typename std::enable_if<
        std::is_same<R, void>::value ||
        std::is_convertible<decltype(std::declval<C>().set(std::declval<A>()...)), R>::value
    >::type
> : public std::true_type {};

} // namespace detail


//placeholder
struct _ {};

template<typename T, typename = void>
struct Mixin : public T
{
private:
    template<typename U>
    using catch_constructible = typename std::enable_if<std::is_constructible<T, U>::value>::type*;

    template<typename U>
    using catch_non_constructible = typename std::enable_if<!std::is_constructible<T, U>::value>::type*;

public:
    template<typename U>
    explicit Mixin(U&& value, catch_constructible<U> = nullptr)
        : T(std::forward<U>(value))
    {}

    template<typename U>
    explicit Mixin(U&&, catch_non_constructible<U> = nullptr)
    {}

    Mixin() = default;
    Mixin(Mixin&&) = default;
    Mixin(const Mixin&) = default;
    Mixin& operator=(Mixin&&) = default;
    Mixin& operator=(const Mixin&) = default;

    template<typename U>
    void set(const U& value)
    {
        doSet(value, detail::is_set_available<T, void(U)>());
    }

private:
    template<typename U>
    void doSet(const U& value, std::true_type)
    {
        T::set(value);
    }

    template<typename U>
    void doSet(const U&, std::false_type)
    {
        // empty
    }
};

template<typename T>
struct Mixin<T, typename std::enable_if<IsBasicType<T>::value || IsCompositeType<T>::value>::type>
{
    template<typename U>
    explicit Mixin(U&&)
    {
        // empty
    }

    Mixin() = default;

    template<typename U>
    void set(U&&)
    {
        // empty
    }
};

template<typename Subject, typename Base>
struct ApplyMixin
{
    using type = Mixin<Subject>;
};

template<template <typename> class Subject, typename Base>
struct ApplyMixin<Subject<_>, Base>
{
    using type = Mixin<Subject<Base>>;
};

template<typename Type, typename... Subject>
struct MixinPolicy : public ApplyMixin<Subject, MixinPolicy<Type, Subject...>>::type...
{
    using type = Type;
    using self_type = Type;

    template<typename T>
    explicit MixinPolicy(const T& value)
        : ApplyMixin<Subject, MixinPolicy>::type(value)...
    {}

    MixinPolicy() = default;
    MixinPolicy(MixinPolicy&&) = default;
    MixinPolicy(const MixinPolicy&) = default;
    MixinPolicy& operator=(MixinPolicy&&) = default;
    MixinPolicy& operator=(const MixinPolicy&) = default;

    template<typename T>
    void set(const T& value)
    {
        doSet<T, Subject...>(value);
    }

    template<typename T>
    static Type* self(T* object) { return static_cast<Type*>(object); }

    template<typename T>
    static const Type* self(const T* object) { return static_cast<const Type*>(object); }

private:
    template<typename T, typename U1, typename U2, typename... U>
    void doSet(const T& value)
    {
        doSet<T,U1>(value);
        doSet<T,U2,U...>(value);
    }

    template<typename T, typename U1>
    void doSet(const T& value)
    {
        static_cast<typename ApplyMixin<U1, MixinPolicy>::type*>(this)->set(value);
    }
};

} // namespace di

