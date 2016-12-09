#pragma once

//local
#include "presence_policy.hpp"

//boost
#include <boost/optional.hpp>

namespace di
{

template<typename TagType, typename ValueType, typename... Policies>
struct BasicType
    : private GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>
{
    using value_type = ValueType;
    using presence_policy = typename GetPolicy<PresencePolicy, Policies...>::template Apply<value_type>;

    BasicType() = default;
    explicit BasicType(value_type&& val) 
        : presence_policy(true)
        , value(std::move(val)) 
    {}
    explicit BasicType(const value_type& val)
        : presence_policy(true)
        , value(val) 
    {}
    
    explicit operator bool() const { return initialized(); }
    const value_type& get() const { return value; }

    using presence_policy::initialized;

protected:
    value_type value;
};

} // namespace di

