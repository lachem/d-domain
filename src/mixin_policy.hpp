//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//std
#include <utility>
#include <type_traits>

namespace di {

//placeholder
struct _ {};

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

template<typename T>
struct MixinType : public T
{
private:
    template<typename U>
    using catch_constructible = typename std::enable_if<std::is_constructible<T, U>::value>::type*;

    template<typename U>
    using catch_non_constructible = typename std::enable_if<!std::is_constructible<T, U>::value>::type*;

public:
    MixinType() = default;

    template<typename U>
    explicit MixinType(U&& value, catch_constructible<U> = nullptr)
        : T(std::forward<U>(value))
    {}

    template<typename U>
    explicit MixinType(U&&, catch_non_constructible<U> = nullptr)
    {}

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

template<typename Subject, typename Base>
struct ApplyMixin
{
    using type = MixinType<Subject>;
};

template<template <typename> class Subject, typename Base>
struct ApplyMixin<Subject<_>, Base>
{
    using type = MixinType<Subject<Base>>;
};

template<class... Subject>
struct Mixin
{
    template<typename Type>
    struct Apply : public ApplyMixin<Subject, Apply<Type>>::type...
    {
        Apply() = default;

        template<typename T>
        explicit Apply(const T& value)
            : ApplyMixin<Subject, Apply<Type>>::type(value)...
        {}

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
            static_cast<typename ApplyMixin<U1, Apply<Type>>::type*>(this)->set(value);
        }
    };
};

} // namespace di

