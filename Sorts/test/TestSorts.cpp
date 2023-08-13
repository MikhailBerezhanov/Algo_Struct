#include "BubbleSort.hpp"
#include "InsertSort.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <vector>

using namespace ::testing;
using namespace AlgoStruct;

TEST(TestSorts, ShouldBubbleSortTwOelements)
{
    std::vector vec{12, 3};

    BubbleSort(vec);

    ASSERT_THAT(vec, ElementsAreArray({3, 12}));
}

TEST(TestSorts, ShouldBubbleSortVectorInNonDescendingOrder)
{
    std::vector vec{2, 3, 1, 5, 4};

    BubbleSort(vec);

    ASSERT_THAT(vec, ElementsAreArray({1, 2, 3, 4, 5}));
}

TEST(TestSorts, ShouldInsertSortVectorInNonDescendingOrder)
{
    std::vector vec{2.2, 3.3, -1.1, 5.5, 4.4, 0.0, 2.2, -10.3};

    InsertSort(vec);

    ASSERT_THAT(vec, ElementsAreArray({-10.3, -1.1, 0.0, 2.2, 2.2, 3.3, 4.4, 5.5}));
}