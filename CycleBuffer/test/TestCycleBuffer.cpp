#include <CycleBuffer.hpp>

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

using namespace ::testing;
using namespace AlgoStruct;


TEST(TestCycleBuffer, ShouldPushBack)
{
    CycleBuffer<int> sut(3);
    ASSERT_TRUE(sut.empty());

    for (int i = 1; i <=3; ++i) {
        sut.push_back(i);
        ASSERT_EQ(i, sut.back());
    }
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());

    const std::vector expectedVec1{1, 2, 3};
    ASSERT_EQ(expectedVec1, sut.GetContent());

    sut.push_back(10);
    ASSERT_EQ(10, sut.back());
    ASSERT_EQ(3, sut.size());

    const std::vector expectedVec2{2, 3, 10};
    ASSERT_EQ(expectedVec2, sut.GetContent());
}

TEST(TestCycleBuffer, ShouldPushFront)
{
    
}


