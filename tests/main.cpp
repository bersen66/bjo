#include <adder/adder.hpp>
#include <gtest/gtest.h>
#include <iostream>

TEST(TestADDING, test1) {
  EXPECT_EQ(adder::Add(2, 1), 3);
  EXPECT_EQ(adder::Add(3, 1), 4);
  EXPECT_EQ(adder::Add(3, 5), 8);
}

TEST(TestADDING, test2) {
  EXPECT_EQ(adder::Add(-1, 1), 0);
  EXPECT_EQ(adder::Add(-3, -1), -4);
  EXPECT_EQ(adder::Add(3, -5), -2);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
