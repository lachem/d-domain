//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include "traits.hpp"

//std
#include <type_traits>

namespace di {
namespace detail {

template<typename T, typename = void>
struct ValueOf
{
    using init = std::true_type;

    template<typename P>
    explicit ValueOf(P&& param)
        : value(std::forward<P>(param))
    {
    }

    ValueOf() = default;
    ValueOf(ValueOf&&) = default;
    ValueOf(const ValueOf&) = default;
    ValueOf& operator=(ValueOf&&) = default;
    ValueOf& operator=(const ValueOf&) = default;

    T value;
};

template<typename T>
struct ValueOf<T, typename std::enable_if<!IsBasicType<T>::value && !IsCompositeType<T>::value>::type>
{
    using init = std::false_type;
};

template<typename T1, typename... T>
struct ValueType
    : public ValueOf<T1>
    , public ValueType<T...>
{
    template<typename... P>
    explicit ValueType(P&&... params)
        : ValueType(typename ValueOf<T1>::init(), std::forward<P>(params)...)
    {
    }

    template<typename P1, typename... P>
    explicit ValueType(std::true_type, P1&& param1, P&&... params)
        : ValueOf<T1>(std::forward<P1>(param1))
        , ValueType<T...>(std::forward<P>(params)...)
    {
    }

    template<typename... P>
    explicit ValueType(std::false_type, P&&... params)
        : ValueType<T...>(std::forward<P>(params)...)
    {
    }

    ValueType() = default;
    ValueType(ValueType&&) = default;
    ValueType(const ValueType&) = default;
    ValueType& operator=(ValueType&&) = default;
    ValueType& operator=(const ValueType&) = default;
};

template<typename T1>
struct ValueType<T1>
    : public ValueOf<T1>
{
    template<typename P1>
    explicit ValueType(P1&& param1)
        : ValueOf<T1>(std::forward<P1>(param1))
    {
    }

    ValueType() = default;
    ValueType(ValueType&&) = default;
    ValueType(const ValueType&) = default;
    ValueType& operator=(ValueType&&) = default;
    ValueType& operator=(const ValueType&) = default;
};

template<typename T, typename = void>
struct MixinOf : public T
{
};

template<typename T>
struct MixinOf<T, typename std::enable_if<IsBasicType<T>::value || IsCompositeType<T>::value>::type>
{
};

template<typename T1, typename... T>
struct MixinType
    : public MixinOf<T1>
    , public MixinType<T...>
{
};

template<typename T1>
struct MixinType<T1>
    : public MixinOf<T1>
{
};

} // namespace detail

template<typename... Types>
struct CompositeType
    : private detail::ValueType<Types...>
    , public detail::MixinType<Types...>
{
    using tag = CompositeTypeTag;

    template<typename... P>
    explicit CompositeType(P&&... params)
        : detail::ValueType<Types...>(std::forward<P>(params)...)
    {
    }

    CompositeType() = default;
    CompositeType(CompositeType&&) = default;
    CompositeType(const CompositeType&) = default;
    CompositeType& operator=(CompositeType&&) = default;
    CompositeType& operator=(const CompositeType&) = default;

    template<typename T>
    void set(T&& field)
    {
        using type = detail::ValueOf<typename std::decay<T>::type>;
        static_cast<type*>(this)->value = std::forward<T>(field);
    }

    template<typename T>
    T& get()
    {
        using type = detail::ValueOf<T>;
        return static_cast<type*>(this)->value;
    }

    template<typename T>
    const T& get() const
    {
        using type = detail::ValueOf<T>;
        return static_cast<const type*>(this)->value;
    }
};

} // namespace di

