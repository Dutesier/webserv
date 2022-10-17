#include <gtest/gtest.h>
#include "my_code.hpp"

TEST(MyCodeTests, IsGtestRunning) {
    my_code();
    EXPECT_EQ(0, 0);
}

TEST(MyCodeTests, MyCodeFuncReturns0) {
    ASSERT_EQ(my_code(), 0);
}



int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}