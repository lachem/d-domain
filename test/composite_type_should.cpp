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
#include <string>
#include <functional>

using namespace di;
using namespace testing;

namespace {

struct NameTag {};
struct PropertyTag {};

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
struct BasicHash
{
    size_t hash() const
    {
       return std::hash<typename T::type::value_type>()(T::self(this)->get());
    }
};

using Name     = BasicType<std::string, NameTag, BasicHash<di::_>>;
using Property = BasicType<uint64_t, PropertyTag, BasicHash<di::_>>;

//------- Static Mixins -------
template<typename T>
struct FixedHash
{
    size_t hash() const
    {
        return 42;
    }
};

TEST(CompositeTypeShould, supportStaticMixins)
{
    using NamedProperty = CompositeType<Name, FixedHash<di::_>, Property>;

    NamedProperty property(Name("SomeName"), Property(2010));
    EXPECT_EQ("SomeName", property.get<Name>().get());
    EXPECT_EQ(2010, property.get<Property>().get());
    EXPECT_EQ(42, property.hash());
}

//------- Dynamic Mixins -------
template<typename T>
struct HashSum
{
    size_t hash() const
    {
        return T::self(this)->template get<Name>().hash()
             + T::self(this)->template get<Property>().hash();
    }
};

using NamedProperty = CompositeType<Name, HashSum<di::_>, Property>;

TEST(CompositeTypeShould, supportDynamicMixins)
{
    Name name("SomeName");
    Property property(2020);
    NamedProperty namedProperty(name, property);

    EXPECT_EQ(name.hash() + property.hash(), namedProperty.hash());
}

} // namespace

