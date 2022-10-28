#include <gtest/gtest.h>
#include "smt.hpp"
#include <iostream>
#include <memory>

TEST(SmartPointerTests, TestUniquePtr) {
    int* numberPtr = new int(42);

    smt::unique_ptr<int> intUniquePtr(numberPtr);
    std::cout << *intUniquePtr << std::endl;
}