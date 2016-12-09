#include <gmock/gmock.h>

#include <basic_type.hpp>

using namespace di;
using namespace testing;

namespace 
{

struct BasicTypeShould : Test
{
};

TEST_F(BasicTypeShould, beDefaultConstructible)
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

} // namespace 

