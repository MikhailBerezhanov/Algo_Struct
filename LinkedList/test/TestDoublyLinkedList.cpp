#include "DoublyLinkedList.hpp"

#include <gtest/gtest.h>

using namespace ::testing;
using namespace AlgoStruct;

template<typename T>
std::string ToString(const DoublyLinkedList<T>& sut)
{
    std::ostringstream oss;
    oss << '[';
    for (const auto elem : sut)
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

TEST(TestDoublyLinkedList, ShouldEditBackElement)
{
    DoublyLinkedList sut{10, 20};

    sut.back() = 100;
    ASSERT_EQ(100, sut.back());
    ASSERT_EQ(10, sut.front());
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeHead)
{
    DoublyLinkedList sut{1, 2, 7, 9};

    sut.insert(sut.begin(), 0);
    ASSERT_EQ(0, sut.front());
    ASSERT_EQ(5, sut.size());

    const DoublyLinkedList expected{0, 1, 2, 7, 9};
    ASSERT_EQ(expected, sut) << "SUT: " << ToString(sut);
}

TEST(TestDoublyLinkedList, ShouldInsertBetweenElements)
{
    
}

TEST(TestDoublyLinkedList, ShouldInsertBeforeTail)
{

}

TEST(TestDoublyLinkedList, ShouldThrowAtInsertionBeforeEnd)
{

}