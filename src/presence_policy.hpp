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
        explicit Apply(bool) {}

        bool initialized() const { return true; }
    };
};

struct Optional : PresencePolicy
{   
    template<typename ValueType>
    struct Apply
    {
        Apply() = default;
        explicit Apply(bool i) : init(i) {}

        bool initialized() const { return static_cast<bool>(init); }

    private:
        bool init = false;
    };
};

template<>
struct StaticDefault<PresencePolicy>
{
    using type = Optional;
};

} // namespace di

