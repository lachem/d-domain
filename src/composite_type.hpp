//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

namespace di {

template<typename... Types>
struct CompositeType : private Types...
{
    template<typename... P>
    explicit CompositeType(P&&... params)
        : P(params)...
    {
    }

    template<typename T>
    void set(T&& field)
    {
        using type = typename std::decay<T>::type;
        *static_cast<type*>(this) = std::forward<T>(field);
    }

    template<typename T>
    T& get()
    {
        return *static_cast<T*>(this);
    }

    template<typename T>
    const T& get() const
    {
        return *static_cast<const T*>(this);
    }
};

} // namespace di

