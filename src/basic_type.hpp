//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include "value_container.hpp"
#include "mixin_policy.hpp"
#include "access_policy.hpp"
#include "presence_policy.hpp"

//boost
#include <boost/optional.hpp>

//std
#include <utility>

namespace di {

template<typename ValueType, template <typename> class... Policies>
struct BasicType
    : private ValueContainer<ValueType>
    , public MixinPolicy<Policies...>::template Apply<BasicType<ValueType, Policies...>>
{
    using value_type = ValueType;
    using value_container = ValueContainer<ValueType>;
    using mixin_policy = typename MixinPolicy<Policies...>::template Apply<BasicType<ValueType, Policies...>>;

    BasicType() = default;
    explicit BasicType(const value_type& val)
        : value_container(val)
        , mixin_policy(val)
    {}
    explicit BasicType(value_type&& val)
        : value_container(std::move(val))
        , mixin_policy(ValueContainer<ValueType>::value)
    {}

    template<typename T>
    void set(T&& val)
    {
        value_container::value = val;
    }

    const value_type& get() const
    {
        return value_container::value;
    }
};

} // namespace di

