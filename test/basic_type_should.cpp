//          Copyright Adam Lach 2016
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// test
#include <gmock/gmock.h>

// ddomain
#include <basic_type.hpp>

using namespace di;
using namespace testing;

namespace
{

struct BasicTypeShould : Test
{
};

template<typename T>
struct SomeTypeTag
{
};

TEST_F(BasicTypeShould, beDefaultConstructibleByDefault)
{
    using SomeType = BasicType<int, SomeTypeTag>;

    SomeType instance;
//    EXPECT_FALSE(instance.initialized());
}

TEST_F(BasicTypeShould, retriveTheValueThatWasStoredAsOptional)
{
    using SomeType = BasicType<int, Optional, SomeTypeTag>;

    SomeType instance(1);
    EXPECT_EQ(1, instance.get());
    EXPECT_TRUE(instance.initialized());
}

TEST_F(BasicTypeShould, retriveTheValueThatWasStoredAsRequired)
{
    using SomeType = BasicType<int, Required, SomeTypeTag>;

    SomeType instance(1);
    EXPECT_EQ(1, instance.get());
    EXPECT_TRUE(instance.initialized());
}

TEST_F(BasicTypeShould, beCopyConstructible)
{
    using OptSomeType = BasicType<int, Optional, SomeTypeTag>;
    using ReqSomeType = BasicType<int, Required, SomeTypeTag>;

    OptSomeType opt = OptSomeType(10);
    ReqSomeType req = ReqSomeType(21);

    EXPECT_EQ(10, opt.get());
    EXPECT_EQ(21, req.get());

    opt = OptSomeType(37);
    req = ReqSomeType(41);

    EXPECT_EQ(37, opt.get());
    EXPECT_EQ(41, req.get());
}

TEST_F(BasicTypeShould, beMovable)
{
    using OptSomeType = BasicType<std::string, Optional>;
    using ReqSomeType = BasicType<std::string, Required>;

    std::string text1("text1");
    std::string text2("text2");

    OptSomeType opt(std::move(text1));
    ReqSomeType req(std::move(text2));

    EXPECT_EQ("text1", opt.get());
    EXPECT_EQ("text2", req.get());

    OptSomeType opt2(std::string("text3"));
    ReqSomeType req2(std::string("text4"));

    opt = std::move(opt2);
    req = std::move(req2);

    EXPECT_EQ("text3", opt.get());
    EXPECT_EQ("text4", req.get());
}

TEST_F(BasicTypeShould, beMutable)
{
    using OptSomeType = BasicType<std::string, Optional>;
    using ReqSomeType = BasicType<std::string, Mutable, Required>;

    std::string text1("text1");
    std::string text2("text2");

    OptSomeType opt(std::move(text1));
    ReqSomeType req(std::move(text2));

    EXPECT_EQ("text1", opt.get());
    EXPECT_EQ("text2", req.get());

    opt.set(std::string("text3"));
    req.set(std::string("text4"));

    EXPECT_EQ("text3", opt.get());
    EXPECT_EQ("text4", req.get());
}

TEST_F(BasicTypeShould, handleMutableAndOptionalTogeather)
{
    using OptSomeType = BasicType<std::string, Optional, Mutable>;

    OptSomeType opt;
    opt.set("text1");

    EXPECT_EQ("text1", opt.get());
    EXPECT_TRUE(opt.initialized());
}

template<typename T>
struct Name
{
    static const char* name() { return "some name"; }
};

TEST_F(BasicTypeShould, supportCustomMixins)
{
    using OptSomeType = BasicType<std::string, Optional, Mutable, Name>;

    OptSomeType opt("text1");

    EXPECT_EQ("text1", opt.get());
    EXPECT_STREQ("some name", opt.name());
}

template<typename T>
struct Prefix
{
    std::string prefix() const
    {
        return T::self(this)->get().substr(0,6);
    }
};

TEST_F(BasicTypeShould, supportValueBasedMixins)
{
    using OptSomeType = BasicType<std::string, Prefix>;

    OptSomeType opt("prefix_text");

    EXPECT_EQ("prefix_text", opt.get());
    EXPECT_EQ("prefix", opt.prefix());
}

} // namespace

