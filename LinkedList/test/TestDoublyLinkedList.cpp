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

TEST(TestDoublyLinkedList, ShouldPushBackElements)
{
    DoublyLinkedList<int> sut;
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());

    sut.push_back(1);
    sut.push_back(2);
    sut.push_back(3);

    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(3, sut.size());

    ASSERT_EQ(1, sut.front());
    ASSERT_EQ(3, sut.back());
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
    // TODO
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeHead)
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
    DoublyLinkedList sut{-2, -1, 0, 2, 3};


}

TEST(TestDoublyLinkedList, ShouldInsertBeforeTail)
{

}

TEST(TestDoublyLinkedList, ShouldThrowAtInsertionBeforeEnd)
{

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