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
    template<typename Type>
    struct Apply
    {
        Apply() = delete;
        explicit Apply(bool) {}
        
        template<typename T> 
        void operator()(const T&) {}

        bool initialized() const { return true; }
    };
};

struct Optional : PresencePolicy
{   
    template<typename Type>
    struct Apply
    {
        Apply() = default;
        explicit Apply(bool i) : init(i) {}

        template<typename T> 
        void operator()(const T&) { init = true; }

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

