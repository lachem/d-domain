//          Copyright Adam Lach 2019
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <iostream>
#include <cstring>
#include <cctype>

namespace di {
namespace detail {

template<char Character>
constexpr void validate()
{
	static_assert(
		('a' <= Character && Character <= 'z') || 
		('A' <= Character && Character <= 'Z') || 
		('_' == Character), 
		"bad name character, only [a-zA-Z_] are allowed");
}

template<char First, char Second, char... Rest>
constexpr void validate()
{
	validate<First>();
	validate<Second, Rest...>();
}

} // namespace detail

template<char... String>
struct NameMixin
{
	static const char* name() 
	{
		static const char str[] = {String..., '\0'};
		return str;
	}
};

template<typename CharT, CharT ... String>
constexpr NameMixin<String...> operator""_name() 
{
	detail::validate<String...>();
 	return {};
}

} // namespace di

