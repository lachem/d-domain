//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// test
#include <gmock/gmock.h>

// ddomain
#include <basic_type.hpp>
#include <composite_type.hpp>

//std
#include <functional>

using namespace di;
using namespace testing;

namespace {

template<typename T> struct NameTag {};
template<typename T> struct PropertyTag {};

TEST(CompositeTypeShould, provideAccessToUnderlyingTypes)
{
    using Name     = BasicType<std::string, NameTag>;
    using Property = BasicType<uint64_t, PropertyTag>;
    using NamedProperty = CompositeType<Name, Property>;

    NamedProperty property(Name("SomeName"), Property(2010));
    EXPECT_EQ("SomeName", property.get<Name>().get());
    EXPECT_EQ(2010, property.get<Property>().get());
}

TEST(CompositeTypeShould, allowModificationOfMutableElements)
{
    using Name     = BasicType<std::string, Mutable, NameTag>;
    using Property = BasicType<uint64_t, Mutable, PropertyTag>;
    using NamedProperty = CompositeType<Name, Property>;

    NamedProperty property(Name("SomeName"), Property(2010));
    EXPECT_EQ("SomeName", property.get<Name>().get());
    EXPECT_EQ(2010, property.get<Property>().get());

    property.set(Name("NewName"));
    EXPECT_EQ("NewName", property.get<Name>().get());
    EXPECT_EQ(2010, property.get<Property>().get());
}

template<typename T>
struct Hashable
{
    size_t hash() const
    {
       return std::hash<typename T::value_type>(T::self(this)->get());
    }
};

TEST(CompositeTypeShould, supportUniversalMixins)
{
    using Name     = BasicType<std::string, NameTag, Hashable>;
    using Property = BasicType<uint64_t, PropertyTag, Hashable>;
    using NamedProperty = CompositeType<Name, Property>;

    NamedProperty property(Name("SomeName"), Property(2010));
    EXPECT_EQ("SomeName", property.get<Name>().get());
    EXPECT_EQ(2010, property.get<Property>().get());
}

} // namespace

