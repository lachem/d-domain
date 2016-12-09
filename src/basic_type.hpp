#pragma once

//boost
#include <boost/optional.hpp>

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

struct PresencePolicy {};
struct Required : PresencePolicy
{
    template<typename ValueType>
    struct Apply
    {
        Apply() = delete;
        explicit Apply(const ValueType& value) : value(value) {}
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
        explicit Apply(const ValueType& value) : value(value) {}
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

