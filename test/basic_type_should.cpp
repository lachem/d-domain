#include <gmock/gmock.h>

#include <basic_type.hpp>

using namespace di;
using namespace testing;

namespace 
{

struct BasicTypeShould : Test
{
};

TEST_F(BasicTypeShould, beDefaultConstructibleByDefault)
{
    struct SomeTypeTag; using SomeType = BasicType<SomeTypeTag, int>;

    SomeType instance;
    EXPECT_FALSE(static_cast<bool>(instance));
} 

TEST_F(BasicTypeShould, retriveTheValueThatWasStoredAsOptional)
{
    struct SomeTypeTag; using SomeType = BasicType<SomeTypeTag, int, Optional>;

    SomeType instance(1);
    EXPECT_EQ(1, instance.get());
    EXPECT_TRUE(instance.initialized());
} 

TEST_F(BasicTypeShould, retriveTheValueThatWasStoredAsRequired)
{
    struct SomeTypeTag; using SomeType = BasicType<SomeTypeTag, int, Required>;

    SomeType instance(1);
    EXPECT_EQ(1, instance.get());
    EXPECT_TRUE(instance.initialized());
} 

TEST_F(BasicTypeShould, beCopyConstructible)
{
    struct SomeTypeTag; 
    using OptSomeType = BasicType<SomeTypeTag, int, Optional>;
    using ReqSomeType = BasicType<SomeTypeTag, int, Required>;

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
    struct SomeTypeTag; 
    using OptSomeType = BasicType<SomeTypeTag, std::string, Optional>;
    using ReqSomeType = BasicType<SomeTypeTag, std::string, Required>;

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
    struct SomeTypeTag; 
    using OptSomeType = BasicType<SomeTypeTag, std::string, Optional>;
    using ReqSomeType = BasicType<SomeTypeTag, std::string, Mutable, Required>;

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
    struct SomeTypeTag; 
    using OptSomeType = BasicType<SomeTypeTag, std::string, Optional, Mutable>;

    OptSomeType opt;
    opt.set("text1");

    EXPECT_EQ("text1", opt.get());
    EXPECT_TRUE(opt.initialized()); 

    // The below code checks for immutability
    // using OptImmutable = BasicType<SomeTypeTag, std::string, Optional, Immutable>;
    // OptImmutable imm;
    // imm.set("text1");
}

TEST_F(BasicTypeShould, supportStaticMixins)
{
    struct Name
    {
        static const char* name() { return "some name"; }
    };

    struct SomeTypeTag; 
    using OptSomeType = BasicType<SomeTypeTag, std::string, Optional, Mutable, Mixin<Name>>;

    OptSomeType opt("text1");

    EXPECT_EQ("text1", opt.get());
    EXPECT_STREQ("some name", opt.name());
}

TEST_F(BasicTypeShould, supportRuntimeMixins)
{
    struct Name
    {
        explicit Name(const std::string& name) : text(name) {}
        const char* name() { return text.c_str(); }

    private:
        std::string text;
    };

    struct SomeTypeTag; 
    using ReqSomeType = BasicType<SomeTypeTag, double, Required, Immutable, Mixin<Name>>;

    ReqSomeType req(15.79, Name("test"));

    EXPECT_EQ(15.79, req.get());
    EXPECT_STREQ("test",  req.name());
}

TEST_F(BasicTypeShould, supportMultipleRuntimeMixins)
{
    struct Name
    {
        explicit Name(const std::string& name) : text(name) {}
        const char* name() { return text.c_str(); }

    private:
        std::string text;
    };

    struct Mode
    {
        explicit Mode(int p) : value(p) {}
        int mode() { return value; }

    private:
        int value;
    };

    struct SomeTypeTag; 
    using ReqSomeType = BasicType<SomeTypeTag, double, Required, Mutable, Mixin<Mode, Name>>;

    ReqSomeType req(15.79, Mode(20), Name("test"));

    EXPECT_EQ(15.79, req.get());
    EXPECT_EQ(20,  req.mode());
    EXPECT_STREQ("test",  req.name());
}

} // namespace 

