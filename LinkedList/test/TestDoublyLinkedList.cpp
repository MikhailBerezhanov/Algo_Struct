#include "DoublyLinkedList.hpp"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace AlgoStruct;

template<typename T>
std::string ToString(const DoublyLinkedList<T>& list)
{
    std::ostringstream oss;
    oss << '[';
    for (const auto elem : list)
    {
        oss << elem << ' ';
    }
    oss << ']';

    return oss.str();
}

TEST(TestDoublyLinkedList, ShouldPushBackOneElement)
{
    DoublyLinkedList<int> sut;
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());

    constexpr auto testVal = 1234567890;
    sut.push_back(1234567890);
    ASSERT_EQ(testVal, sut.front());
    ASSERT_EQ(testVal, sut.back());
}

TEST(TestDoublyLinkedList, ShouldPushBackSeveralElements)
{
    DoublyLinkedList<int> sut{0};

    sut.push_back(5);
    sut.push_back(-7);
    sut.push_back(9);

    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(4, sut.size());
    ASSERT_EQ(0, sut.front());
    ASSERT_EQ(9, sut.back());

    const DoublyLinkedList expected{0, 5, -7, 9};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);

    const int expectedReversed[] = {9, -7, 5, 0};
    auto idx = 0;
    for (auto it = sut.rbegin(); it != sut.rend(); it++)
    {
        ASSERT_EQ(expectedReversed[idx], *it);
        ++idx;
    }
}

TEST(TestDoublyLinkedList, ShouldPushBackManyElements)
{
    DoublyLinkedList<int> sut{0, 0, 100};

    for (auto i = 0; i < 10'000; ++i)
    {
        sut.push_back(i);
    }

    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(10'003, sut.size());
    ASSERT_EQ(0, sut.front());
    ASSERT_EQ(9999, sut.back());
}

TEST(TestDoublyLinkedList, ShouldPushFrontOneElement)
{
    DoublyLinkedList<float> sut;

    constexpr auto testVal = -0.993f;
    sut.push_front(testVal);
    ASSERT_EQ(1, sut.size());
    ASSERT_FLOAT_EQ(testVal, sut.front());
    ASSERT_FLOAT_EQ(testVal, sut.back());
}

TEST(TestDoublyLinkedList, ShouldPushFrontkSeveralElements)
{
    DoublyLinkedList<int> sut;

    sut.push_front(-3);
    sut.push_front(-2);
    sut.push_front(-1);

    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());
    ASSERT_EQ(-1, sut.front());
    ASSERT_EQ(-3, sut.back());

    const DoublyLinkedList expected{-1, -2, -3};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);

    auto val = -3;
    for (auto it = sut.rbegin(); it != sut.rend(); it++)
    {
        ASSERT_EQ(val, *it);
        ++val;
    }
}

TEST(TestDoublyLinkedList, ShouldReturnRendAfterPushBack)
{
    DoublyLinkedList<int> sut;

    constexpr auto testVal = 10;
    sut.push_back(testVal);

    ASSERT_EQ(testVal, *sut.rbegin());
}

TEST(TestDoublyLinkedList, ShouldReturnRendAfterPushFront)
{
    DoublyLinkedList<int> sut;

    constexpr auto testVal = -10;
    sut.push_front(testVal);

    ASSERT_EQ(testVal, *sut.rbegin());
}

TEST(TestDoublyLinkedList, ShouldPushFrontManyElements)
{
    DoublyLinkedList<float> sut{1.1f, 2.43f, -0.06f};

    for (auto i = 0; i < 10'000; ++i)
    {
        sut.push_front(i * 0.1f);
    }
    ASSERT_EQ(10'003, sut.size());
    ASSERT_FLOAT_EQ(999.9f, *sut.begin());
}

TEST(TestDoublyLinkedList, ShouldThrowAtFrontOnEmptyList)
{
    DoublyLinkedList<int> sut;
    ASSERT_ANY_THROW(sut.front());
}

TEST(TestDoublyLinkedList, ShouldThrowAtBackOnEmptyList)
{
    DoublyLinkedList<int> sut;
    ASSERT_ANY_THROW(sut.back());
}

TEST(TestDoublyLinkedList, ShouldEditBackElement)
{
    DoublyLinkedList sut{10, 20};

    sut.back() = 100;
    ASSERT_EQ(100, sut.back());
    ASSERT_EQ(10, sut.front());
}

TEST(TestDoublyLinkedList, ShouldReturnEqualBeginAndEndOnEmptyList)
{
    DoublyLinkedList<char> sut;
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(TestDoublyLinkedList, ShouldReturnNotEqualBeginAndEnd)
{
    DoublyLinkedList<char> sut{'a', 'a'};
    ASSERT_NE(sut.begin(), sut.end());
}

TEST(TestDoublyLinkedList, ShouldCompareEqualLists)
{
    DoublyLinkedList<char> sut1{'a', 'B', '4'};
    DoublyLinkedList<char> sut2{'a', 'B', '4'};
    ASSERT_EQ(sut1, sut2) << "SUT1: " << ToString(sut1) << ", SUT2: " << ToString(sut2);
}

TEST(TestDoublyLinkedList, ShouldCompareNotEqualLists)
{
    {
        DoublyLinkedList<char> sut1{'a', 'B', '4'};
        DoublyLinkedList<char> sut2{'a', 'B', 'C'};
        ASSERT_NE(sut1, sut2) << "SUT1: " << ToString(sut1) << ", SUT2: " << ToString(sut2);
    }
    {
        DoublyLinkedList<char> sut1{'a'};
        DoublyLinkedList<char> sut2;
        ASSERT_NE(sut1, sut2) << "SUT1: " << ToString(sut1) << ", SUT2: " << ToString(sut2);
    }
}

TEST(TestDoublyLinkedList, ShouldClearAllElements)
{
    DoublyLinkedList sut{1, -2, -3, 4, 5};

    ASSERT_EQ(5, sut.size());
    ASSERT_FALSE(sut.empty());
    ASSERT_NE(sut.begin(), sut.end());

    sut.clear();
    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(TestDoublyLinkedList, ShouldClearMultipleTimesSafely)
{
    DoublyLinkedList sut{1, -2, -3, 4, 5};

    sut.clear();
    sut.clear();
    sut.clear();
    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());
}

TEST(TestDoublyLinkedList, ShouldPushBackAfterClear)
{
    DoublyLinkedList sut{1, -2, -3, 4, 5};

    sut.clear();
    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());

    sut.push_back(100);
    sut.push_back(243);
    sut.push_back(-987);

    const DoublyLinkedList expected{100, 243, -987};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeHeadOnEmptyList)
{
    DoublyLinkedList<char> sut;

    const auto it = sut.insert(sut.begin(), 'a');
    ASSERT_EQ('a', *it);
    ASSERT_EQ('a', sut.front());
    ASSERT_EQ(1, sut.size());

    ASSERT_EQ('b', *sut.insert(sut.begin(), 'b'));
    ASSERT_EQ('c', *sut.insert(sut.begin(), 'c'));
    ASSERT_EQ(3, sut.size());
    ASSERT_EQ('c', sut.front());
    ASSERT_EQ('a', sut.back());

    const DoublyLinkedList expected{'c', 'b', 'a'};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeEndOnEmptyList)
{
    DoublyLinkedList<int> sut;
    ASSERT_EQ(0, sut.size());

    {
        const auto it = sut.insert(sut.end(), 999);
        ASSERT_EQ(999, *it);
        ASSERT_EQ(1, sut.size());
        ASSERT_EQ(999, sut.back());
    }
    {
        const auto it = sut.insert(sut.end(), -100);
        ASSERT_EQ(-100, *it);
        ASSERT_EQ(2, sut.size());
        ASSERT_EQ(999, sut.front());
        ASSERT_EQ(-100, sut.back());
    }
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeBegin)
{
    DoublyLinkedList sut{1, 2, 7, 9};

    const auto it = sut.insert(sut.begin(), 0);
    ASSERT_EQ(0, *it);
    ASSERT_EQ(0, sut.front());
    ASSERT_EQ(5, sut.size());

    const DoublyLinkedList expected{0, 1, 2, 7, 9};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldInsertBetweenElements)
{
    DoublyLinkedList sut{0, 1, 2};

    const auto it = sut.begin();
    sut.push_front(-1);
    sut.push_front(-2);
    ASSERT_NE(it, sut.begin());
    ASSERT_EQ(0, *it);

    const auto it2 = sut.insert(it, 777);
    ASSERT_EQ(777, *it2);
    const DoublyLinkedList expected{-2, -1, 777, 0, 1, 2};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldTraverseListInReversedOrder)
{
    DoublyLinkedList sut{5, 4, 3, 2, 1, 0};

    auto expected = 0;
    for (auto it = sut.rbegin(); it != sut.rend(); ++it)
    {
        ASSERT_EQ(expected, *it);
        ++expected;
    }
}

TEST(TestDoublyLinkedList, ShouldEraseElementFromBegin)
{
    DoublyLinkedList sut{1, 2, 3};

    sut.erase(sut.begin());
    ASSERT_EQ(2, sut.front());
    ASSERT_EQ(2, sut.size());

    sut.erase(sut.begin());
    ASSERT_EQ(3, sut.front());
    ASSERT_EQ(1, sut.size());

    sut.erase(sut.begin());
    ASSERT_TRUE(sut.empty());
}


TEST(TestDoublyLinkedList, ShouldEraseElementFromEnd)
{
    DoublyLinkedList sut{1000, -100, 7123400};

    sut.erase(--sut.end());

    const DoublyLinkedList expected{1000, -100};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldEraseElementFromMiddle)
{
    DoublyLinkedList sut{10, 20};

    const auto it = sut.insert(sut.end(), 30);
    sut.push_back(40);
    sut.erase(it);

    const DoublyLinkedList expected{10, 20, 40};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldPushBackAfterErase)
{
    DoublyLinkedList sut{-3020};

    sut.erase(sut.begin());
    sut.push_back(10);
    sut.push_back(100);
    sut.push_front(6123);

    const DoublyLinkedList expected{6123, 10, 100};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);

    const int expectedReversed[] = {100, 10, 6123};
    auto idx = 0;
    for (auto it = sut.rbegin(); it != sut.rend(); ++it)
    {
        ASSERT_EQ(expectedReversed[idx], *it);
        ++idx;
    }
}

TEST(TestDoublyLinkedList, ShouldPushFrontAfterErase)
{
    DoublyLinkedList sut{-88, 123};

    sut.erase(sut.begin());
    sut.push_front(60);
    sut.push_front(120);
    sut.push_back(-333);
    sut.push_back(0);

    const DoublyLinkedList expected{120, 60, 123, -333, 0};
    ASSERT_EQ(expected, sut) << "Expected: " << ToString(expected) << ", got: " << ToString(sut);

    const int expectedReversed[] = {0, -333, 123, 60, 120};
    auto idx = 0;
    for (auto it = sut.rbegin(); it != sut.rend(); ++it)
    {
        ASSERT_EQ(expectedReversed[idx], *it);
        ++idx;
    }
}

TEST(TestDoublyLinkedList, ShouldPopBackOneElement)
{
    
}

TEST(TestDoublyLinkedList, ShouldPopBackSeveralElements)
{
    
}

TEST(TestDoublyLinkedList, ShouldPopFrontkOneElement)
{
    
}

TEST(TestDoublyLinkedList, ShouldPopFrontSeveralElements)
{
    
}

TEST(TestDoublyLinkedList, ShouldPushBackAfterPopFront)
{
    
}

TEST(TestDoublyLinkedList, ShouldPushFrontAfterPopBack)
{
    
}