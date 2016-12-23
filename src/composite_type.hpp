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

template<size_t>
struct Empty {};

template<typename T>
struct ValueOf
{
    T value;
};

template<typename T1, typename... T>
struct ValueType
    : public std::conditional
            < IsBasicType<T1>::value || IsCompositeType<T1>::value
            , ValueOf<T1>
            , Empty<sizeof...(T)>>::type
    , public ValueType<T...>
{
};

template<typename T1>
struct ValueType<T1>
    : public std::conditional
            < IsBasicType<T1>::value || IsCompositeType<T1>::value
            , ValueOf<T1>
            , Empty<0>>::type
{
};

template<typename T1, typename... T>
struct MixinType
    : public std::conditional
        < !IsBasicType<T1>::value && !IsCompositeType<T1>::value
        , T1
        , Empty<sizeof...(T)>>::type
    , public MixinType<T...>
{
};

template<typename T1>
struct MixinType<T1>
    : public std::conditional
        < !IsBasicType<T1>::value && !IsCompositeType<T1>::value
        , T1
        , Empty<0>>::type
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
    explicit CompositeType(P&&... /*params*/)
//        : detail::ValueType<P>(params)...
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

