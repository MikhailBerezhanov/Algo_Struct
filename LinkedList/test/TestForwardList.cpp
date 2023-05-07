#include "ForwardList.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;
using namespace AlgoStruct;

TEST(TestForwardList, ShouldFillList)
{
    ForwardList list;

    for (int i = 4; i >= 0; --i)
    {
        list.push_front(i);
    }

    const auto values = traverse(list);

    ASSERT_THAT(values, ElementsAreArray({0, 1, 2, 3, 4}));
}

TEST(TestForwardList, ShouldThrowOnFrontOfEmptyList)
{
    ForwardList list;
    ASSERT_ANY_THROW(list.front());
}

TEST(TestForwardList, ShouldReturnFrontElements)
{
    ForwardList list;
    ASSERT_ANY_THROW(list.front());
}

TEST(TestForwardList, ShouldClearList)
{
    ForwardList list;

    for (int i = 0; i < 5; ++i)
    {
        list.push_front(i);
    }

    list.clear();

    ASSERT_TRUE(list.empty());
}

TEST(TestForwardList, ShouldReverseList) 
{
    ForwardList list;

    // 4 -> 3 -> 2 -> 1 -> 0
    for (int i = 0; i < 5; ++i)
    {
        list.push_front(i);
    }

    list.reverse();

    for (int i = 0; i < 5; ++i)
    {
        ASSERT_EQ(i, list.front());
        list.pop_front();
    }
}