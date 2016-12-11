//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

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

