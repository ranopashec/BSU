#include <gtest/gtest.h>
#include "../include/palindrome.h"

TEST(PalindromeTest, PositiveNumber) {
    EXPECT_TRUE(isPalindrome(121));
    EXPECT_FALSE(isPalindrome(123));
}

TEST(PalindromeTest, NegativeNumber) {
    EXPECT_FALSE(isPalindrome(-121));
}
