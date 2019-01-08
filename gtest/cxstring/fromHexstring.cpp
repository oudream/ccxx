
#include <gtest/gtest.h>

#include <ccxx/cxstring.h>

using namespace std;

namespace
{
// Tests the Increment() method.

    TEST(CxString, fromHexstring9string)
    {
        vector<uchar> data = CxString::fromHexstring("3132336141");
        string s((char*)&data.front(), data.size());
        EXPECT_EQ(s, "123aA");
    }


}  // namespace
