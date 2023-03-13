#include "../src/utils/slice.h"

#include <gtest/gtest.h>

#include <string>

using nanodb::Slice;

TEST(SliceTest, BasicSliceTest) {
    std::string str("This is a std::string.");
    Slice s(str);
    ASSERT_EQ(s.data(), str.data());
    ASSERT_EQ(s.length(), str.length());
    ASSERT_EQ(s[3], 's');
    ASSERT_FALSE(s.Empty());
    s.Clear();
    ASSERT_TRUE(s.Empty());
    s = Slice(str);
    ASSERT_TRUE(s.StartsWith("This"));
    ASSERT_FALSE(s.StartsWith("is"));
    s.RemovePrefix(5);
    ASSERT_FALSE(s.StartsWith("This"));
    ASSERT_TRUE(s.StartsWith("is"));
    ASSERT_EQ(Slice("NanoDB").ToString(), std::string("NanoDB"));
    ASSERT_EQ(Slice("alice").Compare(Slice("bob")), -1);
}