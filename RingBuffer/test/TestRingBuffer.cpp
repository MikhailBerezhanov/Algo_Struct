#include <RingBuffer.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <string>

using namespace R2C;
using namespace ::testing;


template<typename T>
struct ContentParameters
{
    T expectedFirstElement;
    T expectedLastElement;
    T step;
};

template<typename T>
static void CheckBufferContent(const RingBuffer<T>& buf, const ContentParameters<T> &checkParams)
{
    auto startVal = checkParams.expectedFirstElement;
    auto lastVal = startVal;

    for (const auto& elem : buf)
    {
        ASSERT_EQ(startVal, elem);
        lastVal = startVal;
        startVal += checkParams.step;
    }

    ASSERT_EQ(checkParams.expectedLastElement, lastVal);
}

template<typename T>
static void CheckBufferReversedContent(const RingBuffer<T>& buf, const ContentParameters<T> &checkParams)
{
    auto startVal = checkParams.expectedFirstElement;
    auto lastVal = startVal;

    for (auto it = buf.crbegin(); it != buf.crend(); ++it)
    {
        ASSERT_EQ(startVal, *it);
        lastVal = startVal;
        startVal -= checkParams.step;
    }

    ASSERT_EQ(checkParams.expectedLastElement, lastVal);
}

struct FillingParameters
{
    size_t maxSize = 0; 
    size_t numberOfPushes = 0;
    size_t expectedFinalSize = 0;
};

static void TestBufferFilling(const FillingParameters& fillParams, const ContentParameters<int>& checkParams)
{
    auto sut = RingBuffer<int>(fillParams.maxSize);

    for (auto val = 0; val < fillParams.numberOfPushes; ++val)
    {
        sut.push(val);
    }

    ASSERT_EQ(fillParams.expectedFinalSize, sut.size());
    CheckBufferContent(sut, checkParams);
}


TEST(RingBufferTest, ShouldBeEmptyWhenNoPushesPerformed)
{
    const size_t bufferMaxSize = 100;
    auto sut = RingBuffer<int>(bufferMaxSize);

    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(0, sut.size());
}

TEST(RingBufferTest, ShouldReturnFrontElementBeforeBufferRotation)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<int>(bufferMaxSize);

    for (int i = 0; i < 3; ++i)
    {
        sut.push(10 + i);
    }

    ASSERT_EQ(10, sut.front());
    ASSERT_EQ(10, *sut.begin());
}

TEST(RingBufferTest, ShouldReturnFrontElementAfterBufferRotation)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<int>(bufferMaxSize);

    for (int i = 0; i < 11; ++i)
    {
        sut.push(i);
    }

    ASSERT_EQ(1, sut.front());
    ASSERT_EQ(1, *sut.begin());

    sut.push(99);
    ASSERT_EQ(2, sut.front());
    ASSERT_EQ(2, *sut.begin());

    for (int i = 0; i < 8; ++i)
    {
        sut.push(50 + i);
    }

    ASSERT_EQ(10, sut.front());
    ASSERT_EQ(10, *sut.begin());
}

TEST(RingBufferTest, ShouldModifyFrontElement)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<char>(bufferMaxSize);

    sut.push('A');
    sut.front() = 'B';

    ASSERT_EQ('B', *(sut.begin()));
    ASSERT_EQ('B', *(sut.cbegin()));
}

TEST(RingBufferTest, ShouldReturnBackElementBeforeBufferRotation)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<double>(bufferMaxSize);

    for (int i = 0; i < 8; ++i)
    {
        sut.push(11.1 * i);
    }

    ASSERT_DOUBLE_EQ(77.7, sut.back());
}

TEST(RingBufferTest, ShouldReturnBackElementAfterBufferRotation)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<double>(bufferMaxSize);

    for (int i = 0; i < 26; ++i)
    {
        sut.push(5.123 + i);
    }

    ASSERT_DOUBLE_EQ(30.123, sut.back());
}

TEST(RingBufferTest, ShouldPushToBufferUsingMove)
{
    auto sut = RingBuffer<std::string>(30);
    const std::string referenceContent = "abcDEFGhijklM NOPqqqWW  WW12312312312\r\n\t";
    std::string dataToBeMoved = referenceContent;

    sut.push(std::move(dataToBeMoved));

    ASSERT_EQ(1, sut.size());
    ASSERT_EQ(referenceContent, sut.front());
    ASSERT_EQ(referenceContent, sut.back());
}

TEST(RingBufferTest, ShouldPushToBufferUsingCopy)
{
    auto sut = RingBuffer<std::string>(30);
    const std::string constDataToBeCopied = "qwertyZXCXZ CZXCZXCLSADOFS FO12312391239-01293";
    std::string dataToBeCopied = "=-=0-0-9=]'/ .;[pl,mkoijnb huygvcftrddxzzseaaweqwe";

    sut.push(constDataToBeCopied);
    sut.push(dataToBeCopied);

    ASSERT_EQ(2, sut.size());
    ASSERT_EQ(constDataToBeCopied, sut.front());
    ASSERT_EQ(dataToBeCopied, sut.back());

    ASSERT_FALSE(constDataToBeCopied.empty());
    ASSERT_FALSE(dataToBeCopied.empty());
}

TEST(RingBufferTest, ShouldCopyConstruct)
{
    const size_t bufferMaxSize = 10;
    auto tempBuffer = RingBuffer<std::string>(bufferMaxSize);
    const std::string testData = "zxcvasd qwe 123\n\n\n\\\\\"ggffASDQWE\"";

    for (size_t i = 0; i < 13; ++i)
    {
        tempBuffer.push(std::to_string(i) + testData);
    }
    
    RingBuffer sut(tempBuffer);

    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(bufferMaxSize, sut.size());

    auto expectedFirstElement = 3;
    for (const auto& elem : sut)
    {
        ASSERT_EQ(std::to_string(expectedFirstElement) + testData, elem);
        ++expectedFirstElement;
    }

    ASSERT_EQ(tempBuffer, sut);
}

TEST(RingBufferTest, ShouldMoveConstruct)
{
    const size_t bufferMaxSize = 100;
    auto tempBuffer = RingBuffer<std::string>(bufferMaxSize);
    const std::string testData = "string to be pushed to temporary buffer";

    for (size_t i = 0; i < 232; ++i)
    {
        tempBuffer.push(testData + std::to_string(i));
    }
    ASSERT_TRUE(tempBuffer.full());

    RingBuffer sut(std::move(tempBuffer));

    ASSERT_TRUE(sut.full());
    ASSERT_FALSE(sut.empty());
    ASSERT_EQ(bufferMaxSize, sut.size());

    auto expectedFirstElement = 132;
    for (const auto& elem : sut)
    {
        ASSERT_EQ(testData + std::to_string(expectedFirstElement), elem);
        ++expectedFirstElement;
    }
}

TEST(RingBufferTest, ShouldCopyAssign)
{
    auto tempBuffer = RingBuffer<std::string>(30);
    for (size_t i = 0; i < 43; ++i)
    {
        tempBuffer.push(std::to_string(i));
    }

    auto sut = RingBuffer<std::string>(10);
    sut.push("abcdef_____123");
    ASSERT_FALSE(sut.full());

    sut = tempBuffer;

    ASSERT_TRUE(sut.full());
    ASSERT_EQ(30, sut.size());
    ASSERT_EQ(tempBuffer, sut);
}

TEST(RingBufferTest, ShouldMoveAssign)
{
    auto tempBuffer = RingBuffer<std::string>(30);
    for (size_t i = 0; i < 43; ++i)
    {
        tempBuffer.push(std::to_string(i));
    }

    auto sut = RingBuffer<std::string>(50);
    for (size_t i = 0; i < 10; ++i)
    {
        sut.push("test str");
    }
    ASSERT_FALSE(sut.full());

    sut = std::move(tempBuffer);

    ASSERT_EQ(30, sut.size());
    ASSERT_TRUE(sut.full());

    auto expectedFirstElement = 13;
    for (const auto& elem : sut)
    {
        ASSERT_EQ(std::to_string(expectedFirstElement), elem);
        ++expectedFirstElement;
    }
}

TEST(RingBufferTest, ShouldIterateWithConstIterators)
{
    auto sut = RingBuffer<int>(111);

    for (int i = 10; i < 50; ++i)
    {
        sut.push(i);
    }

    const int startValue = 10;
    int expectedFirstElement = startValue;
    for (auto it = sut.cbegin(); it != sut.cend(); ++it)
    {
        ASSERT_EQ(expectedFirstElement++, *it);
    }

    for (auto it = sut.crbegin(); it != sut.crend(); ++it)
    {
        ASSERT_EQ(--expectedFirstElement, *it);
    }

    ASSERT_EQ(startValue, expectedFirstElement);
}

TEST(RingBufferTest, ShouldPartlyFillBuffer)
{
    TestBufferFilling({.maxSize = 1000, .numberOfPushes = 5, .expectedFinalSize = 5}, {.expectedFirstElement = 0, .expectedLastElement = 4, .step = 1});
}

TEST(RingBufferTest, ShouldFullyFillBuffer)
{
    TestBufferFilling({.maxSize = 123, .numberOfPushes = 123, .expectedFinalSize = 123}, {.expectedFirstElement = 0, .expectedLastElement = 122, .step = 1});
}

TEST(RingBufferTest, ShouldFillBufferWithOneElementRotation)
{
    TestBufferFilling({.maxSize = 10, .numberOfPushes = 11, .expectedFinalSize = 10}, {.expectedFirstElement = 1, .expectedLastElement = 10, .step = 1});
}

TEST(RingBufferTest, ShouldFillBufferWithRotation)
{
    TestBufferFilling({.maxSize = 10, .numberOfPushes = 15, .expectedFinalSize = 10}, {.expectedFirstElement = 5, .expectedLastElement = 14, .step = 1});
}

TEST(RingBufferTest, ShouldFillBufferWithoutRotation)
{
    TestBufferFilling({.maxSize = 10, .numberOfPushes = 20, .expectedFinalSize = 10}, {.expectedFirstElement = 10, .expectedLastElement = 19, .step = 1});
}

TEST(RingBufferTest, ShouldFillBufferWithMultipleRotations)
{
    TestBufferFilling({.maxSize = 7, .numberOfPushes = 23, .expectedFinalSize = 7}, {.expectedFirstElement = 16, .expectedLastElement = 22, .step = 1});
}

TEST(RingBufferTest, ShouldFillLargeBufferWithMultipleRotations)
{
    const size_t bufferMaxSize = 500;

    for (int i = 0; i < 3 * bufferMaxSize; ++i)
    {
        TestBufferFilling({.maxSize = bufferMaxSize, .numberOfPushes = (bufferMaxSize + i), .expectedFinalSize = bufferMaxSize},
                          {.expectedFirstElement = i, .expectedLastElement = (static_cast<int>(bufferMaxSize) + i - 1), .step = 1} );
    }
}

TEST(RingBufferTest, ShouldClearBuffer)
{
    auto sut = RingBuffer<int>(1000);

    for (int i = 0; i < 1234; ++i)
    {
        sut.push(i);
    }

    sut.clear();

    ASSERT_TRUE(sut.empty());
    ASSERT_FALSE(sut.full());
    ASSERT_EQ(0, sut.size());
}

TEST(RingBufferTest, ShouldFillBufferAfterClearing)
{
    auto sut = RingBuffer<double>(500);

    for (int i = 0; i < 1000; ++i)
    {
        sut.push(i * -22.2);
    }

    sut.clear();

    ASSERT_TRUE(sut.empty());
    ASSERT_FALSE(sut.full());
    ASSERT_EQ(0, sut.size());

    for (int i = 0; i < 500; ++i)
    {
        sut.push(i * -1.1);
    }

    ASSERT_TRUE(sut.full());
    ASSERT_DOUBLE_EQ(0.0, sut.front());
    ASSERT_DOUBLE_EQ(-548.9, sut.back());

    sut.clear();

    ASSERT_TRUE(sut.empty());
    ASSERT_FALSE(sut.full());
    ASSERT_EQ(0, sut.size());

    for (int i = 0; i < 1200; ++i)
    {
        sut.push(i * 2.0);
    }

    ASSERT_DOUBLE_EQ(1400.0, sut.front());
    ASSERT_DOUBLE_EQ(2398.0, sut.back());
}

TEST(RingBufferTest, ShouldSwapWithOtherBuffer)
{
    const size_t bufferMaxSize = 10;
    auto sut = RingBuffer<int>(bufferMaxSize);

    for (int i = 0; i < 15; ++i)
    {
        sut.push(i);
    }

    ASSERT_EQ(bufferMaxSize, sut.size());

    const size_t otherBufferMaxSize = 2 * bufferMaxSize;
    RingBuffer<int> other(otherBufferMaxSize);

    sut.swap(other);

    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(bufferMaxSize, other.size());

    ASSERT_EQ(5, other.front());
    ASSERT_EQ(14, other.back());

    CheckBufferContent(other, {.expectedFirstElement = 5, .expectedLastElement = 14, .step = 1});

    // Check if maxSize has been changed after swapping
    for (int i = 0; i < otherBufferMaxSize; ++i)
    {
        sut.push(i);
    }

    ASSERT_EQ(otherBufferMaxSize, sut.size());
    ASSERT_EQ(0, sut.front());
    ASSERT_EQ(otherBufferMaxSize - 1, sut.back());
}

TEST(RingBufferTest, ShouldFillBufferAfterStdSwap)
{
    const size_t bufferMaxSize = 100;
    auto sut = RingBuffer<int>(bufferMaxSize);

    for (int i = 0; i < 199; ++i)
    {
        sut.push(i);
    }

    ASSERT_EQ(bufferMaxSize, sut.size());

    const size_t otherBufferMaxSize = 13;
    RingBuffer<int> other(otherBufferMaxSize);

    for (int i = 0; i < 10; ++i)
    {
        other.push(i);
    }

    std::swap(sut, other);

    ASSERT_EQ(bufferMaxSize, other.size());
    ASSERT_EQ(99, other.front());
    ASSERT_EQ(198, other.back());
    CheckBufferContent(other, {.expectedFirstElement = 99, .expectedLastElement = 198, .step = 1});

    ASSERT_EQ(10, sut.size());
    for (int i = 0; i < 5; ++i)
    {
        sut.push(10 + i);
    }

    ASSERT_EQ(2, sut.front());
    ASSERT_EQ(14, sut.back());
    CheckBufferContent(sut, {.expectedFirstElement = 2, .expectedLastElement = 14, .step = 1});
}

TEST(RingBufferTest, ShouldSatisfyForwardIteratorRequirements)
{
    // Semantic requirements:
    // I models std::forward_iterator if, and only if I models all the concepts it subsumes, 
    // and given objects a and b of type I:
    //
    // * Comparison between iterators a and b has a defined result if
    //   **  a and b are iterators to the same underlying sequence, or
    //   **  both a and b are value-initialized, in which case they compare equal.
    //
    // * Pointers and references obtained from a forward iterator into a range remain valid while the range exists.
    //
    // * If a and b are dereferenceable, they offer the multi-pass guarantee, that is:
    //   1) If a and b compare equal (a == b is contextually convertible to true) then either they are both 
    //      non-dereferenceable or *a and *b are references bound to the same object.
    //   2) If *a and *b refer to the same object, then a == b.
    //   3) Assignment through a mutable ForwardIterator iterator cannot invalidate the iterator (implicit due to 
    //      reference defined as a true reference).
    //   4) Incrementing a copy of a does not change the value read from a (formally, either It is a raw pointer 
    //      type or the expression (void)++It(a), *a is equivalent to the expression *a).
    //   5) a == b implies ++a == ++b.

    auto sut = RingBuffer<int>(23);

    // 1.
    ASSERT_TRUE(sut.empty());
    ASSERT_EQ(sut.begin(), sut.end());

    // 2.
    sut.push(1993);
    ASSERT_EQ(sut.begin(), sut.begin());

    for (int i = 0; i < 50; ++i)
    {
        sut.push(i);
    }

    // 3.
    auto a = sut.begin();
    const int mutatedElementValue = -199890;
    *a = mutatedElementValue;
    ASSERT_EQ(mutatedElementValue, sut.front());

    // 4.
    auto aCopy(a);
    ++aCopy;
    ASSERT_TRUE(*aCopy != mutatedElementValue);
    aCopy++;
    ASSERT_TRUE(*aCopy != mutatedElementValue);
    ASSERT_EQ(mutatedElementValue, *a);

    // 5.
    size_t iterationsCounter = 0;
    for (auto b = sut.begin(); a != sut.end(); ++a, ++b)
    {
        ASSERT_EQ(a, b);
        ++iterationsCounter;
    }
    ASSERT_EQ(sut.size(), iterationsCounter);
}

TEST(RingBufferTest, ShouldSatisfyBidirectionalIteratorRequirements)
{
    // Semantic requirements:
    // A bidirectional iterator r is said to be decrementable if and only if there exists some s such that ++s == r.
    //
    // std::bidirectional_iterator<I> is modeled only if all the concepts it subsumes are modeled, 
    // and given two objects a and b of type I:
    //
    // 1) If a is decrementable, a is in the domain of the expressions --a and a--.
    // 2) Pre-decrement yields an lvalue that refers to the operand: std::addressof(--a) == std::addressof(a);
    // 3) Post-decrement yields the previous value of the operand: if bool(a == b), then bool(a-- == b).
    // 4) Post-decrement and pre-decrement perform the same modification on its operand: If bool(a == b), 
    //    then after evaluating both a-- and --b, bool(a == b) still holds.
    // 5) Increment and decrement are inverses of each other:
    //    ** If a is incrementable and bool(a == b), then bool(--(++a) == b).
    //    ** If a is decrementable and bool(a == b), then bool(++(--a) == b).

    auto sut = RingBuffer<char>(5);

    // 2.
    sut.push('R');
    auto it = sut.end();
    ASSERT_EQ('R', *--it);
    ASSERT_EQ('R', *it);
    ASSERT_EQ(it, sut.begin());

    for (size_t i = 0; i < 7; ++i)
    {
        sut.push('a' + i);
    }

    // 3.
    it = sut.end();
    it--;
    ASSERT_EQ('g', *it);
    ASSERT_EQ('g', *it--);
    ASSERT_EQ('f', *it);
    ASSERT_EQ('e', *--it);

    // 4.
    auto a = sut.end();
    auto b = sut.end();
    ASSERT_EQ(a, b);
    a--;
    --b;
    ASSERT_EQ(a, b);

    // 5.
    ASSERT_EQ(--(++a), b);
    ASSERT_EQ(++(--a), b);
}

TEST(RingBufferTest, ShouldApplyStdForEach)
{
    auto sut = RingBuffer<int>(43);

    for (int i = 0; i < 50; ++i)
    {
        sut.push(-1);
    }

    std::for_each(sut.cbegin(), sut.cend(), [](int val)
    {
        ASSERT_EQ(-1, val); 
    });

    std::for_each(sut.begin(), sut.end(), [](int& val)
    {
        val *= -150; 
    });

    std::for_each(sut.cbegin(), sut.cend(), [](int val)
    {
        ASSERT_EQ(150, val); 
    });
}

TEST(RingBufferTest, ShouldApplyStdTransform)
{
    auto sut = RingBuffer<char>(10);

    for (int i = 0; i < 10; ++i)
    {
        sut.push('a' + i);
    }

    std::transform(sut.begin(), sut.end(), sut.begin(), [](char val)
    {
        return std::toupper(val);
    });

    CheckBufferContent(sut, {.expectedFirstElement = 'A', .expectedLastElement = 'J', .step = static_cast<char>(1)});
}

TEST(RingBufferTest, ShouldApplyStdFill)
{
    auto sut = RingBuffer<double>(200);

    for (int i = 0; i < 220; ++i)
    {
        sut.push(-1.23406 * i);
    }

    std::fill(sut.begin(), sut.end(), 0.003);

    for (const auto& elem : sut)
    {
        ASSERT_EQ(0.003, elem);
    }
}

TEST(RingBufferTest, ShouldApplyStdFindIf)
{
    auto sut = RingBuffer<std::pair<std::string, int>>(3);

    sut.push({"first element", 1});
    sut.push({"second element", 2});
    sut.push({"third element", 3});
    sut.push({"fourth element", 4});
    sut.push({"fifth element", 5});

    auto it = std::find_if(sut.cbegin(), sut.cend(), [](const auto& pair)
    {
        return pair.first == "first element";
    });
    ASSERT_TRUE(it == sut.cend());

    it = std::find_if(sut.cbegin(), sut.cend(), [](const auto& pair)
    {
        return pair.first == "fourth element";
    });
    ASSERT_FALSE(it == sut.cend());
    ASSERT_EQ(4, it->second);
}

TEST(RingBufferTest, ShouldIterateInReversedOrderBeforeRotation)
{
    const size_t bufferMaxSize = 15;
    auto sut = RingBuffer<int>(bufferMaxSize);

    for (int i = 0; i < bufferMaxSize; ++i)
    {
        sut.push(i);
    }

    CheckBufferReversedContent(sut, {.expectedFirstElement = bufferMaxSize - 1, .expectedLastElement = 0, .step = 1});
}

TEST(RingBufferTest, ShouldIterateInReversedOrderAfterRotation)
{
    auto sut = RingBuffer<int>(20);

    for (int i = 0; i < 21; ++i)
    {
        sut.push(-i);
    }

    CheckBufferReversedContent(sut, {.expectedFirstElement = -20, .expectedLastElement = -1, .step = -1});
}

TEST(RingBufferTest, ShouldIterateInReversedOrderAfterMultipleRotations)
{
    const int valueStep = 20;
    auto sut = RingBuffer<int>(10);

    for (int i = 0; i < 27; ++i)
    {
        sut.push(i * valueStep);
    }

    CheckBufferReversedContent(sut, {.expectedFirstElement = 520, .expectedLastElement = 340, .step = valueStep});
}
