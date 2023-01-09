#include "SocketConnection.hpp"
#include "TCPSocket.hpp"
#include "smt.hpp"
#include "SocketConnection.hpp"

#include <gtest/gtest.h>
#include <iostream>
#include <memory>
#include <vector>

TEST(SmartPointerTests, TestUniquePtr) {
    int* numberPtr = new int(42);

    smt::unique_ptr<int> intUniquePtr(numberPtr);
    ASSERT_EQ(*intUniquePtr, 42);
}

TEST(SmartPointerTests, TestTwoUniquePtr) {
    int* numberPtr = new int(42);

    smt::unique_ptr<int> intUniquePtr(numberPtr);
    smt::unique_ptr<int> intOtherUniquePtr;

    // The code commented bellow should NOT compile
    // smt::unique_ptr<int> intOtherUniquePtr(intUniquePtr);
    // intOtherUniquePtr = intUniquePtr;
    ASSERT_EQ(*intUniquePtr, 42);
}

TEST(SmartPointerTests, TestSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);

    ASSERT_EQ(*intSharedPtr, 42);
}

TEST(SmartPointerTests, TestTwoSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);
    smt::shared_ptr<int> intOtherSharedPtr(intSharedPtr);

    ASSERT_EQ(*intSharedPtr, 42);
    ASSERT_EQ(*intOtherSharedPtr, 42);
}

TEST(SmartPointerTests, TestVectorSharedPtr) {
    int* numberPtr = new int(42);

    smt::shared_ptr<int> intSharedPtr(numberPtr);
    smt::shared_ptr<int> intOtherSharedPtr(intSharedPtr);

    std::vector<smt::shared_ptr<int> > vec;
    vec.push_back(intSharedPtr);
    vec.push_back(intOtherSharedPtr);
    ASSERT_EQ(*vec.at(0), 42);
    ASSERT_EQ(*vec.at(1), 42);
}

TEST(SmartPointerTests, TestVectorSharedPtrSockets) {
    webserv::SocketAddress* socketPtr = new webserv::SocketAddress();

    smt::shared_ptr<webserv::SocketAddress> SocketAddressSharedPtr(socketPtr);
    smt::shared_ptr<webserv::SocketAddress> SocketAddressOtherSharedPtr(
        SocketAddressSharedPtr);

    std::vector<smt::shared_ptr<webserv::SocketAddress> > vec;
    vec.push_back(SocketAddressSharedPtr);
    vec.push_back(SocketAddressOtherSharedPtr);
    ASSERT_EQ(vec.at(0)->address(), vec.at(1)->address());
}

TEST(SmartPointerTests, TestMakeShared) {
    smt::shared_ptr<webserv::SocketAddress> socketAddress = smt::make_shared<webserv::SocketAddress>();
    ASSERT_TRUE(socketAddress);

}