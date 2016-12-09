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
    using presence_policy = typename GetPolicy<PresencePolicy, Policies...>::template Apply<ValueType>;

    BasicType() = default;
    explicit BasicType(const value_type& value) : presence_policy(value) {}
    
    explicit operator bool() const { return initialized(); }

    using presence_policy::get;
    using presence_policy::initialized;
};

} // namespace di

