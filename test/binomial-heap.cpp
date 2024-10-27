#include "binomial-heap.h"
#include "gtest/gtest.h"

TEST(BinomialHeap, Empty)
{
    BinomialHeap<int> heap;
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(0, heap.size());
}

TEST(BinomialHeap, Push)
{
    BinomialHeap<int> heap;
    heap.push(1);
    EXPECT_FALSE(heap.empty());
    EXPECT_EQ(1, heap.size());
    heap.push(2);
    EXPECT_EQ(2, heap.size());
    heap.push(3);
    EXPECT_EQ(3, heap.size());
}

TEST(BinomialHeap, Pop)
{
    BinomialHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(3);
    EXPECT_EQ(heap.pop(), 1);
    EXPECT_EQ(2, heap.size());
    EXPECT_EQ(heap.pop(), 2);
    EXPECT_EQ(1, heap.size());
    EXPECT_EQ(heap.pop(), 3);
    EXPECT_EQ(0, heap.size());
}

TEST(BinomialHeap, Top)
{
    BinomialHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(3);
    EXPECT_EQ(heap.top(), 1);
    heap.pop();
    EXPECT_EQ(heap.top(), 2);
    heap.pop();
    EXPECT_EQ(heap.top(), 3);
}

TEST(BinomialHeap, Clear)
{
    BinomialHeap<int> heap;
    heap.push(1);
    heap.push(2);
    heap.push(3);
    heap.clear();
    EXPECT_TRUE(heap.empty());
    EXPECT_EQ(0, heap.size());
}

TEST(BinomialHeap, Init)
{
    BinomialHeap<int> heap{1, 2, 3, 4, 5};
    EXPECT_EQ(5, heap.size());
    EXPECT_EQ(1, heap.pop());
    EXPECT_EQ(2, heap.pop());
    EXPECT_EQ(3, heap.pop());
    EXPECT_EQ(4, heap.pop());
    EXPECT_EQ(5, heap.pop());
}

TEST(BinomialHeap, Merge)
{
    BinomialHeap<int> heap1{1, 2, 3};
    BinomialHeap<int> heap2{4, 5, 6};
    heap1.merge(heap2);
    EXPECT_EQ(6, heap1.size());
    EXPECT_EQ(1, heap1.pop());
    EXPECT_EQ(2, heap1.pop());
    EXPECT_EQ(3, heap1.pop());
    EXPECT_EQ(4, heap1.pop());
    EXPECT_EQ(5, heap1.pop());
    EXPECT_EQ(6, heap1.pop());
    EXPECT_TRUE(heap2.empty());
}
