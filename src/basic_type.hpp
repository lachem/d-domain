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

template<typename ValueType, typename... Policies>
struct BasicType
    : private ValueContainer<ValueType>
    , private GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>
    , private GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>
    , public  GetPolicy<MixinPolicy, Policies...>::template Apply<BasicType<ValueType, Policies...>>
{
    using value_type = ValueType;
    using value_container = ValueContainer<ValueType>;
    using presence_policy = typename GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>;
    using access_policy = typename GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>;
    using mixin_policy = typename GetPolicy<MixinPolicy, Policies...>::template Apply<BasicType<ValueType, Policies...>>;

    BasicType() = default;
    explicit BasicType(value_type&& val) 
        : value_container(std::move(val)) 
        , presence_policy(true)
    {}
    explicit BasicType(const value_type& val)
        : value_container(val) 
        , presence_policy(true)
    {}
    template<typename M1, typename... M>
    explicit BasicType(value_type&& val, M1&& mixin1, M&&... mixins) 
        : value_container(std::move(val)) 
        , presence_policy(true)
        , mixin_policy(std::forward<M1>(mixin1), std::forward<M>(mixins)...)
    {}
    template<typename M1, typename... M>
    explicit BasicType(const value_type& val, M1&& mixin1, M&&... mixins)
        : value_container(val) 
        , presence_policy(true)
        , mixin_policy(std::forward<M1>(mixin1), std::forward<M>(mixins)...)
    {}

    using presence_policy::initialized;
   
    explicit operator bool() const 
    { 
        return initialized(); 
    }

    template<typename T>
    void set(T&& val) 
    {
        presence_policy::operator()(val);
        access_policy::operator()(val);
        value_container::value = val;
    }

    const value_type& get() const 
    { 
        return value_container::value; 
    }
};

} // namespace di

