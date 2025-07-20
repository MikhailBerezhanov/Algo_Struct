#include <CycleBuffer.hpp>

#include <gtest/gtest.h>
// #include <gmock/gmock.h>

using namespace ::testing;
using namespace AlgoStruct;

static std::vector<int> CycleBufferContent(CycleBuffer<int>& sut)
{
    std::vector<int> res;
    res.reserve(sut.size());

    for (auto it = sut.begin(); it != sut.end(); ++it) {
        res.push_back(*it);
    }

    return res;
}

TEST(TestCycleBuffer, ShouldPushBack)
{
    constexpr int Size = 4;
    CycleBuffer<int> sut(Size);
    ASSERT_TRUE(sut.empty());

    for (int i = 1; i <=3; ++i) {
        sut.push_back(i);
        ASSERT_EQ(i, sut.back());
    }
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());

    const std::vector expectedVec1{1, 2, 3};
    ASSERT_EQ(expectedVec1, CycleBufferContent(sut));

    sut.push_back(4);
    ASSERT_EQ(4, sut.back());
    ASSERT_EQ(Size, sut.size());

    const std::vector expectedVec2{1, 2, 3, 4};
    ASSERT_EQ(expectedVec2, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldPushFront)
{
    
}


