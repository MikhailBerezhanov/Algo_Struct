#include "ForwardList.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

using namespace ::testing;
using namespace AlgoStruct;

// Helper functions
template <typename T>
std::vector<T> traverse(const ForwardList<T>& list)
{
    std::vector<T> values;

    for (const auto& elem : list)
    {
        values.push_back(elem);
    }

    return values;
}

TEST(TestForwardList, ShouldPushFrontTheList)
{
    ForwardList<int> list;

    for (int i = 4; i >= 0; --i)
    {
        list.push_front(i);
    }

    ASSERT_EQ(list.size(), 5);
    ASSERT_THAT(traverse(list), ElementsAreArray({0, 1, 2, 3, 4}));
}

TEST(TestForwardList, ShouldPushBackTheList)
{
    ForwardList<int> list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_back(i);
    }

    const auto values = traverse(list);

    ASSERT_EQ(list.size(), 5);
    ASSERT_THAT(values, ElementsAreArray({0, 1, 2, 3, 4}));
}

TEST(TestForwardList, ShouldPushBackAndPushFrontTheList)
{
    ForwardList<int> list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_back(i);
        list.push_front(i);
    }

    const auto values = traverse(list);

    ASSERT_EQ(list.size(), 10);
    ASSERT_THAT(values, ElementsAreArray({4, 3, 2, 1, 0, 0, 1, 2, 3, 4}));
}

TEST(TestForwardList, ShouldThrowOnFrontOfEmptyList)
{
    ForwardList<int> list;
    ASSERT_ANY_THROW(list.front());
}

TEST(TestForwardList, ShouldPopFrontElements)
{
    ForwardList<int> list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_front(i);
    }

    for (int i = 4; i >= 0; --i)
    {
        ASSERT_EQ(list.front(), i);
        list.pop_front();
    }

    ASSERT_EQ(list.size(), 0);
}

TEST(TestForwardList, ShouldClearList)
{
    ForwardList<int> list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_front(i);
    }

    list.clear();

    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(list.empty());
}

TEST(TestForwardList, ShouldReverseTheList) 
{
    ForwardList<int> list;

    // 4 -> 3 -> 2 -> 1 -> 0
    for (int i = 0; i < 5; ++i)
    {
        list.push_front(i);
    }

    list.reverse();
    list.push_back(6);
    list.push_front(-1);

    ASSERT_THAT(traverse(list), ElementsAreArray({-1, 0, 1, 2, 3, 4, 6}));
}

TEST(TestForwardList, ShouldInsertAfterHead)
{
    ForwardList<int> list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_back(i);
    }

    auto it = list.begin();
    list.insert_after(it, 10);
    ASSERT_THAT(traverse(list), ElementsAreArray({0, 10, 1, 2, 3, 4}));
}

TEST(TestForwardList, ShouldInsertAfterTail)
{
    ForwardList list{0, 1, 2, 3, 4};

    auto it = list.begin();
    it += 4;
    list.insert_after(it, 10);

    ASSERT_THAT(traverse(list), ElementsAreArray({0, 1, 2, 3, 4, 10}));

    list.push_back(11);
    ASSERT_THAT(traverse(list), ElementsAreArray({0, 1, 2, 3, 4, 10, 11}));
}

TEST(TestForwardList, ShouldIgnoreInsertingAfterEnd)
{
    ForwardList list{1};

    auto it = list.end();
    list.insert_after(it, 2);

    ASSERT_EQ(list.size(), 1);
    ASSERT_EQ(list.front(), 1);
    ASSERT_EQ(list.back(), 1);
}

TEST(TestForwardList, ShouldEraseAfterBegin)
{
    ForwardList list{-1, -2, 0, 3, 3};

    auto it = list.begin();
    auto it2 = list.erase_after(it);

    ASSERT_EQ(*it2, 0);
    ASSERT_THAT(traverse(list), ElementsAreArray({-1, 0, 3, 3}));
}

TEST(TestForwardList, ShouldEraseTail)
{
    ForwardList list{0, 0, 0, 1};

    auto it = list.begin();
    std::advance(it, 2);
    auto it2 = list.erase_after(it);

    ASSERT_EQ(it2, list.end());
    ASSERT_THAT(traverse(list), ElementsAreArray({0, 0, 0}));

    list.push_back(13);
    ASSERT_EQ(list.back(), 13);
}

TEST(TestForwardList, ShouldSortListOfOneElement)
{
    ForwardList list{1};
    list.sort();
    ASSERT_THAT(traverse(list), ElementsAreArray({1}));
}

TEST(TestForwardList, ShouldSortListOfTwoElements)
{
    ForwardList list{2, 0};
    list.sort();
    ASSERT_THAT(traverse(list), ElementsAreArray({0, 2}));
}

TEST(TestForwardList, ShouldSortChars)
{
    ForwardList list{'d', 'b', 'c', 'a', 'e'};
    list.sort();
    ASSERT_THAT(traverse(list), ElementsAreArray({'a', 'b', 'c', 'd', 'e'}));
}

TEST(TestForwardList, ShouldSortWithDuplicates)
{
    ForwardList list{10, 2, 3, 4, 2, -1, 3, 21, 5, 9};
    list.sort();
    ASSERT_THAT(traverse(list), ElementsAreArray({-1, 2, 2, 3, 3, 4, 5, 9, 10, 21}));
}
