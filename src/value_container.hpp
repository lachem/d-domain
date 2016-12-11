//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include <utility>

namespace di {

template<typename ValueType>
struct ValueContainer
{
    ValueContainer() = default;
    ValueContainer(ValueType&& val) : value(std::move(val)) {}
    ValueContainer(const ValueType& val) : value(val) {}
    ValueType value;
};

} // namespace di

