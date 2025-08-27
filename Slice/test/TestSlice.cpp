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

TEST(TestSlice, ShouldPerfromIteratorArithmetics)
{
    Slice sut{1, 2, 3};
    ASSERT_LT(sut.begin(), sut.end());

    auto it = sut.begin();
    ASSERT_EQ(1, it[0]);
    ASSERT_EQ(2, it[1]);
    ASSERT_EQ(3, it[2]);

    it = it + 1;
    it = 1 + it;
    ASSERT_EQ(3, *it);

    auto it2 = it - 1;
    ASSERT_EQ(2, *it2);

    const auto diff = it - sut.begin();
    ASSERT_EQ(2, diff); 
}

TEST(TestlSice, ShouldCopyConstruct)
{
    // TODO
}

TEST(TestlSice, ShouldCopyAssign)
{
    // TODO
}

TEST(TestlSice, ShouldMoveConstruct)
{
    Slice sut1{1, 2, 3};
    Slice sut2(std::move(sut1));

    ASSERT_TRUE(sut1.empty());
    ASSERT_EQ(3, sut2.size());
    ASSERT_EQ(1, sut2.front());
    ASSERT_EQ(2, sut2[1]);
    ASSERT_EQ(3, sut2.back());
}

TEST(TestlSice, ShouldMoveAssign)
{
    Slice sut1{-10, -20, -30, -40, -50};
    Slice sut2{1, 2};

    sut2 = std::move(sut1);
    sut2.push_back(100);
    ASSERT_TRUE(sut1.empty());

    ASSERT_EQ(6, sut2.size());
    ASSERT_EQ(-10, sut2[0]);
    ASSERT_EQ(-20, sut2[1]);
    ASSERT_EQ(-30, sut2[2]);
    ASSERT_EQ(-40, sut2[3]);
    ASSERT_EQ(-50, sut2[4]);
    ASSERT_EQ(100, sut2[5]);
}

