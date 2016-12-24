//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//std
#include <type_traits>

namespace di {

struct BasicTypeTag {};
struct CompositeTypeTag {};

namespace detail {

template<typename T>
struct void_
{
  typedef void type;
};

} // namespace detail

template<typename T, typename Tag, typename U = void>
struct IsType
    : std::false_type
{};

template<typename T, typename Tag>
struct IsType<T, Tag, typename detail::void_<typename T::tag>::type>
    : std::is_same<typename T::tag, Tag>
{};

template<typename T>
using IsBasicType = IsType<T, BasicTypeTag>;

template<typename T>
using IsCompositeType = IsType<T, CompositeTypeTag>;

} // namespace di

