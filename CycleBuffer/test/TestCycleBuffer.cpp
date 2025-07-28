#include <CycleBuffer.hpp>

#include <cstddef>
#include <iterator>

#include <gtest/gtest.h>

using namespace ::testing;
using namespace AlgoStruct;

static std::vector<int> CycleBufferContent(CycleBuffer<int>& sut)
{
    std::vector<int> res;
    res.reserve(sut.size());

    for (auto elem : sut) {
        res.push_back(elem);
    }

    return res;
}

TEST(TestCycleBuffer, ShouldReturnBeginAndEndAtEmptyContainer)
{
    CycleBuffer<int> sut(10);
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());

    const auto begIt = sut.begin();
    const auto endIt = sut.end();
    ASSERT_EQ(begIt, endIt);
}

TEST(TestCycleBuffer, ShouldReturnBeginAfterPushBack)
{
    CycleBuffer<int> sut(3);
    sut.push_back(1);
    ASSERT_EQ(1, *sut.begin());

    sut.push_back(2);
    sut.push_back(3);
    ASSERT_EQ(1, *sut.begin());

    sut.push_back(4);
    ASSERT_EQ(2, *sut.begin());
}

TEST(TestCycleBuffer, ShouldReturnBeginAfterPushFront)
{
    CycleBuffer<int> sut(3);
    sut.push_front(-1);
    ASSERT_EQ(-1, *sut.begin());

    sut.push_front(-2);
    ASSERT_EQ(-2, *sut.begin());

    sut.push_front(0);
    ASSERT_EQ(0, *sut.begin());

    sut.push_front(4);
    ASSERT_EQ(4, *sut.begin());
}

TEST(TestCycleBuffer, ShouldPushBack)
{
    CycleBuffer<int> sut(4);
    ASSERT_TRUE(sut.empty());

    for (int i = 1; i <= 3; ++i) {
        sut.push_back(i);
        ASSERT_EQ(i, sut.back());
    }
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());

    const std::vector expectedContent1{1, 2, 3};
    ASSERT_EQ(expectedContent1, CycleBufferContent(sut));

    sut.push_back(4);
    ASSERT_EQ(4, sut.back());
    ASSERT_EQ(4, sut.size());

    const std::vector expectedContent2{1, 2, 3, 4};
    ASSERT_EQ(expectedContent2, CycleBufferContent(sut));

    sut.push_back(10);
    sut.push_back(20);
    const std::vector expectedContent3{3, 4, 10, 20};
    ASSERT_EQ(expectedContent3, CycleBufferContent(sut));

    sut.push_back(100);
    sut.push_back(-100);
    sut.push_back(-1);
    const std::vector expectedContent4{20, 100, -100, -1};
    ASSERT_EQ(expectedContent4, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldPushBackAlotElements)
{
    CycleBuffer<int> sut(10);

    for (int i = 1; i <= 10'000; ++i) {
        sut.push_back(i);
    }

    ASSERT_EQ(10, sut.size());
    const std::vector expectedContent{9991, 9992, 9993, 9994, 9995, 9996 , 9997, 9998, 9999, 10'000};
    ASSERT_EQ(expectedContent, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldPushFront)
{
    CycleBuffer<int> sut(5);
    ASSERT_TRUE(sut.empty());

    for (int i = 1; i <= 3; ++i) {
        sut.push_front(i);
        ASSERT_EQ(i, sut.front());
    }
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());

    const std::vector expectedContent1{3, 2, 1};
    ASSERT_EQ(expectedContent1, CycleBufferContent(sut));

    sut.push_front(-1);
    sut.push_front(-2);
    const std::vector expectedContent2{-2, -1, 3, 2, 1};
    ASSERT_EQ(expectedContent2, CycleBufferContent(sut));

    sut.push_front(10);
    sut.push_front(20);
    const std::vector expectedContent3{20, 10, -2, -1, 3};
    ASSERT_EQ(expectedContent3, CycleBufferContent(sut));

    sut.push_front(3);
    sut.push_front(2);
    sut.push_front(1);
    sut.push_front(0);
    const std::vector expectedContent4{0, 1, 2, 3, 20};
    ASSERT_EQ(expectedContent4, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldPushFrontAlotElements)
{
    CycleBuffer<int> sut(7);

    for (int i = 1; i <= 10'000; ++i) {
        sut.push_back(-i);
    }

    ASSERT_EQ(7, sut.size());
    const std::vector expectedContent{-9994, -9995, -9996 , -9997, -9998, -9999, -10'000};
    ASSERT_EQ(expectedContent, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldPushBackAndPushFront)
{
    CycleBuffer<int> sut(5);

    sut.push_back(2);
    sut.push_front(-2);
    ASSERT_EQ(2, sut.size());
    ASSERT_EQ(2, sut.back());
    ASSERT_EQ(-2, sut.front());
    const std::vector expectedContent1{-2, 2};
    ASSERT_EQ(expectedContent1, CycleBufferContent(sut));

    sut.push_back(10);
    sut.push_front(-10);
    ASSERT_EQ(4, sut.size());
    ASSERT_EQ(10, sut.back());
    ASSERT_EQ(-10, sut.front());
    const std::vector expectedContent2{-10, -2, 2, 10};
    ASSERT_EQ(expectedContent2, CycleBufferContent(sut));

    sut.push_back(666);
    sut.push_front(777);
    ASSERT_EQ(5, sut.size());
    const std::vector expectedContent3{777, -10, -2, 2, 10};
    ASSERT_EQ(expectedContent3, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldChangeFrontElementViaBeginIterator)
{
    CycleBuffer<int> sut(5);
    sut.push_back(11);
    ASSERT_EQ(11, sut.front());
    ASSERT_EQ(11, sut.back());

    auto it = sut.begin();
    *it = -100;
    ASSERT_EQ(-100, sut.front());
    ASSERT_EQ(-100, sut.back());
    ASSERT_EQ(1, sut.size());
    const std::vector expectedContent{-100};
    ASSERT_EQ(expectedContent, CycleBufferContent(sut));
}

TEST(TestCycleBuffer, ShouldReturnReversedBegin)
{
    CycleBuffer<int> sut(5);

    sut.push_front(-1);
    ASSERT_EQ(-1, *sut.rbegin());

    sut.push_back(10);
    ASSERT_EQ(10, *sut.rbegin());
        
    sut.push_back(-2);
    ASSERT_EQ(-2, *sut.rbegin());

    //      t     h
    // 10, -2, _, 4, -1 
    sut.push_front(4);
    ASSERT_EQ(-2, *sut.rbegin());

    // //  t  h
    // // 10, 7, 3, 4, -1 
    sut.push_front(3);
    sut.push_front(7);
    ASSERT_EQ(10, *sut.rbegin());

    sut.push_back(99);
    ASSERT_EQ(99, *sut.rbegin());
}

TEST(TestCycleBuffer, ShouldIterateContainerInReverseOrder)
{
    CycleBuffer<int> sut(5);
    ASSERT_EQ(sut.rend(), sut.rbegin());

    for (int i = 1; i <= 5; ++i) {
        sut.push_back(i * 10);
    }

    std::vector<int> reversedContent;
    reversedContent.reserve(sut.size());
    for (auto it = sut.rbegin(); it != sut.rend(); ++it) {
        reversedContent.push_back(*it);
    }

    const std::vector<int> expectedReversedContent{50, 40, 30, 20, 10};
    ASSERT_EQ(expectedReversedContent, reversedContent);
}