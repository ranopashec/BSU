#include <gtest/gtest.h>
#include "../include/fibonacci.h"

TEST(FibonacciTest, PositiveN) {
    std::vector<int> expected = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};
    std::vector<int> result = generateFibonacci(10);
    EXPECT_EQ(result, expected);
}

TEST(FibonacciTest, ZeroN) {
    EXPECT_THROW(generateFibonacci(0), std::invalid_argument);
}

TEST(FibonacciTest, NegativeN) {
    EXPECT_THROW(generateFibonacci(-5), std::invalid_argument);
}
