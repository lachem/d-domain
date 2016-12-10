#pragma once

//local
#include "mixin_policy.hpp"
#include "access_policy.hpp"
#include "presence_policy.hpp"

//boost
#include <boost/optional.hpp>

//std
#include <utility>

namespace di
{

template<typename TagType, typename ValueType, typename... Policies>
struct BasicType
    : private GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>
    , private GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>
    , public  GetPolicy<MixinPolicy, Policies...>::template Apply<ValueType>
{
    using value_type = ValueType;
    using presence_policy = typename GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>;
    using access_policy = typename GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>;
    using mixin_policy = typename GetPolicy<MixinPolicy, Policies...>::template Apply<ValueType>;

    BasicType() = default;
    explicit BasicType(value_type&& val) 
        : presence_policy(true)
        , value(std::move(val)) 
    {}
    explicit BasicType(const value_type& val)
        : presence_policy(true)
        , value(val) 
    {}
    template<typename T>
    explicit BasicType(value_type&& val, T&& mixin) 
        : presence_policy(true)
        , mixin_policy(std::forward<T>(mixin))
        , value(std::move(val)) 
    {}
    template<typename T>
    explicit BasicType(const value_type& val, T&& mixin)
        : presence_policy(true)
        , mixin_policy(std::forward<T>(mixin))
        , value(val) 
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
        value = val;
    }

    const value_type& get() const 
    { 
        return value; 
    }

protected:
    value_type value;
};

} // namespace di

