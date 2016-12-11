//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//std
#include <type_traits>

namespace di
{

template<typename Fallback, typename To, typename From1 = void, typename... From>
struct ExtractConvertible
{
    using type = typename std::conditional
        < !std::is_convertible<From1, To>::value
        , typename ExtractConvertible<Fallback, To, From...>::type
        , From1 
        >::type;
};

template<typename Fallback, typename To>
struct ExtractConvertible<Fallback, To, void>
{
    using type = Fallback;
};

template<typename T>
struct StaticDefault;

template<typename Policy, typename... Candidates>
using GetPolicy = typename ExtractConvertible<typename StaticDefault<Policy>::type, Policy, Candidates...>::type;

} // namespace di

