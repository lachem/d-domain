//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//test
#include <gmock/gmock.h>

//ddomain
#include <basic_type.hpp>
#include <composite_type.hpp>

//boost
#include <boost/functional/hash.hpp>

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
       return std::hash<typename T::type::value_type>()(static_cast<const T*>(this)->get());
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
        return static_cast<const T*>(this)->template get<Name>().hash()
             + static_cast<const T*>(this)->template get<Property>().hash();
    }
};

TEST(CompositeTypeShould, supportDynamicMixins)
{
    using NamedProperty = CompositeType<Name, HashSum<di::_>, Property>;

    Name name("SomeName");
    Property property(2020);
    NamedProperty namedProperty(name, property);

    EXPECT_EQ(name.hash() + property.hash(), namedProperty.hash());
}

//------- Visitors -------------
using NamedProperty = CompositeType<Name, HashSum<di::_>, Property>;
using MultiLevel = CompositeType<NamedProperty, Property>;

TEST(CompositeTypeShould, supportMultiLevelHierarchies)
{
    MultiLevel multi(
        NamedProperty(
            Name("SomeName"), Property(2020)),
        Property(42));
    EXPECT_EQ("SomeName", multi.get<NamedProperty>().get<Name>().get());
    EXPECT_EQ(2020, multi.get<NamedProperty>().get<Property>().get());
    EXPECT_EQ(42, multi.get<Property>().get());
}

//------------------------------
struct Visitor
{
    template<typename T>
    void operator()(const T& obj) const
    {
        static_assert(!std::is_same<T,T>::value, "should not get here");
    }

    void operator()(const NamedProperty& obj) const
    {
        EXPECT_EQ("SomeName", obj.get<Name>().get());
        EXPECT_EQ(2020, obj.get<Property>().get());
    }

    void operator()(const Property& obj) const
    {
        EXPECT_EQ(42, obj.get());
    }
};

TEST(CompositeTypeShould, metaVisitor)
{
    MultiLevel multi(
        NamedProperty(
            Name("SomeName"), Property(2020)),
        Property(42));

    multi.accept(Visitor());
}

//------------------------------
struct RecursiveVisitor
{
    template<typename T>
    void operator()(const T& obj) const
    {
        (*this)(obj, typename T::tag());
    }

    template<typename T>
    void operator()(const T& obj, di::CompositeTypeTag) const
    {
        obj.accept(*this);
    }

    template<typename T>
    void operator()(const T&, di::BasicTypeTag) const
    {
        static_assert(!std::is_same<T,T>::value, "should not get here");
    }

    void operator()(const Name& obj) const
    {
        EXPECT_EQ("SomeName", obj.get());
    }

    void operator()(const Property& obj) const
    {
        EXPECT_TRUE(
            obj.get() == 2020 ||
            obj.get() == 42
        );
    }
};

TEST(CompositeTypeShould, recursiveMetaVisitor)
{
    MultiLevel multi(
        NamedProperty(
            Name("SomeName"), Property(2020)),
        Property(42));

    multi.accept(RecursiveVisitor());
}

//------ Universal Hash --------
struct HashFunctor
{
    explicit HashFunctor(size_t* value)
        : value(value)
    {}

    template<typename T>
    void operator()(const T& obj) const
    {
        (*this)(obj, typename T::tag());
    }

    template<typename T>
    void operator()(const T& obj, di::CompositeTypeTag) const
    {
        obj.accept(*this);
    }

    template<typename T>
    void operator()(const T& obj, di::BasicTypeTag) const
    {
        boost::hash_combine(*value, obj.get());
    }

private:
    size_t* value;
};

template<typename T>
struct UniversalHash
{
    size_t hash() const
    {
        size_t value = 0;
        static_cast<const T*>(this)->accept(HashFunctor(&value));
        return value;
    }
};

TEST(CompositeTypeShould, supportUniversalHash)
{
    using Name = BasicType<std::string, UniversalHash<di::_>>;

    size_t hash = 0;
    boost::hash_combine(hash, std::string("text"));

    Name name("text");
    EXPECT_EQ(hash, name.hash());
}

TEST(CompositeTypeShould, supportUniversalHash2)
{
    using Name  = BasicType<std::string>;
    using Value = BasicType<uint64_t>;
    using NamedValue = CompositeType<Name, Value>;
    using MultiLevel = CompositeType<Name, UniversalHash<di::_>, NamedValue, Value>;

    size_t hash = 0;
    boost::hash_combine(hash, std::string("Multi"));
    boost::hash_combine(hash, std::string("Ext"));
    boost::hash_combine(hash, uint64_t(1681));
    boost::hash_combine(hash, uint64_t(491));

    MultiLevel ml(
        Name("Multi"),
        NamedValue(
            Name("Ext"),
            Value(1681)),
        Value(491));
    EXPECT_EQ(hash, ml.hash());
}

} // namespace

