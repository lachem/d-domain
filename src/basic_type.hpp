#pragma once

//local
#include "access_policy.hpp"
#include "presence_policy.hpp"

//boost
#include <boost/optional.hpp>

namespace di
{

template<typename TagType, typename ValueType, typename... Policies>
struct BasicType
    : private GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>
    , private GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>
{
    using value_type = ValueType;
    using presence_policy = typename GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>;
    using access_policy = typename GetPolicy<AccessPolicy, Policies...>::template Apply<ValueType>;

    BasicType() = default;
    explicit BasicType(value_type&& val) 
        : presence_policy(true)
        , value(std::move(val)) 
    {}
    explicit BasicType(const value_type& val)
        : presence_policy(true)
        , value(val) 
    {}
 
    using presence_policy::initialized;
   
    explicit operator bool() const 
    { 
        return initialized(); 
    }

    template<typename T>
    void set(const T& val) 
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

