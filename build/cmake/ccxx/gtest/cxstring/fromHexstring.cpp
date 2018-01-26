
#include <gtest/gtest.h>

#include <ccxx/cxstring.h>

namespace
{
// Tests the Increment() method.

    TEST(CxString, toHexstring9pchar)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9puchar)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9vector7char)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9vector7uchar)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9char)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9uchar)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9short)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9ushort)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
        EXPECT_EQ(CxString::fromHexstring(9), "0x00000009");
        EXPECT_EQ(CxString::fromHexstring(10), "0x0000000A");
        EXPECT_EQ(CxString::fromHexstring(11), "0x0000000B");
        EXPECT_EQ(CxString::fromHexstring(13), "0x0000000D");
    }

    TEST(CxString, toHexstring9int)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9uint)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9int64)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9uint64)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9float)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9double)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9int8bool)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }

    TEST(CxString, toHexstring9uint8bool)
    {
        EXPECT_EQ(CxString::fromHexstring(0), "0x00000000");
        EXPECT_EQ(CxString::fromHexstring(1), "0x00000001");
    }


}  // namespace
