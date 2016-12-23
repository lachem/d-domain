//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

//local
#include "mixin_policy.hpp"
#include "access_mixin.hpp"
#include "presence_mixin.hpp"
#include "traits.hpp"

//boost
#include <boost/optional.hpp>

//std
#include <utility>

namespace di {
namespace detail {

template<typename ValueType>
struct ValueContainer
{
    ValueContainer() = default;
    ValueContainer(ValueType&& val) : value(std::move(val)) {}
    ValueContainer(const ValueType& val) : value(val) {}
    ValueType value;
};

} // namespace detail

template<typename ValueType, typename... Policies>
struct BasicType
    : private detail::ValueContainer<ValueType>
    , public MixinPolicy<BasicType<ValueType, Policies...>, Policies...>
{
    using tag = BasicTypeTag;
    using value_type = ValueType;
    using value_container = detail::ValueContainer<ValueType>;
    using mixin_policy = MixinPolicy<BasicType<ValueType, Policies...>, Policies...>;

    BasicType() = default;
    explicit BasicType(const value_type& val)
        : value_container(val)
        , mixin_policy(val)
    {}
    explicit BasicType(value_type&& val)
        : value_container(std::move(val))
        , mixin_policy(value_container::value)
    {}

    BasicType(BasicType&&) = default;
    BasicType(const BasicType&) = default;

    BasicType& operator=(BasicType&& rhs)
    {
        mixin_policy::operator=(std::move(static_cast<mixin_policy&&>(rhs)));
        value_container::value = std::move(rhs.value_container::value);

        return *this;
    }

    BasicType& operator=(const BasicType& rhs)
    {
        if (&rhs != this)
        {
            mixin_policy::operator=(static_cast<const mixin_policy&>(rhs));
            value_container::value = rhs.value_container::value;
        }

        return *this;
    }

    template<typename T>
    void set(T&& val)
    {
        mixin_policy::set(val);
        value_container::value = val;
    }

    const value_type& get() const
    {
        return value_container::value;
    }
};

} // namespace di

