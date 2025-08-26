#include "Slice.hpp"

#include <gtest/gtest.h>

#include <vector> // TO REMOVE

using namespace ::testing;
using namespace AlgoStruct;

TEST(TestlSice, ShouldPushBack)
{
    Slice<int> sut;

    ASSERT_EQ(0, sut.size());
    ASSERT_TRUE(sut.empty());

    sut.push_back(1);
    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(2, sut.capacity());

    sut.push_back(2);
    ASSERT_EQ(2, sut.size());
    ASSERT_EQ(2, sut.capacity());

    sut.push_back(3);
    ASSERT_EQ(3, sut.size());
    ASSERT_EQ(6, sut.capacity());

    for (auto i = 0; i < sut.size(); ++i)
    {
        EXPECT_EQ(i + 1, sut[i]);
    }
}

TEST(TestlSice, ShouldPushBackToNonEmptySliceAfterConstruction)
{
    Slice sut{1, 2, 3, 4, 5, 6, 7, 8, 9, 0};

    sut.push_back(133);
    ASSERT_EQ(133, sut.back());
}

TEST(TestlSice, ShouldPopBack)
{
    Slice sut{1, 2, 3};

    ASSERT_EQ(3, sut.back());

    sut.pop_back();
    ASSERT_EQ(2, sut.size());
    ASSERT_EQ(2, sut.back());

     sut.pop_back();
    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(1, sut.back());

    sut.pop_back();
    ASSERT_TRUE(sut.empty());
}