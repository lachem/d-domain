#pragma once

//local
#include "policy.hpp"

//boost
#include <boost/optional.hpp>

namespace di
{

struct PresencePolicy {};
struct Required : PresencePolicy
{
    template<typename ValueType>
    struct Apply
    {
        Apply() = delete;
        explicit Apply(ValueType&& val) : value(std::move(val)) {}
        explicit Apply(const ValueType& val) : value(val) {}
        bool initialized() const { return true; }
        const ValueType& get() const { return value; }
        ValueType value;
    };
};

struct Optional : PresencePolicy
{   
    template<typename ValueType>
    struct Apply
    {
        Apply() = default;
        explicit Apply(ValueType&& val) : value(std::move(val)) {}
        explicit Apply(const ValueType& val) : value(val) {}
        bool initialized() const { return static_cast<bool>(value); }
        const ValueType& get() const { return *value; }
        boost::optional<ValueType> value;
    };
};

template<>
struct StaticDefault<PresencePolicy>
{
    using type = Optional;
};

} // namespace di

