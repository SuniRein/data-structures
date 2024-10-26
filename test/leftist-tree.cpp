#include "leftist-tree.h"
#include "gtest/gtest.h"

TEST(LeftistTree, Empty)
{
    LeftistTree<int> tree;
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(0, tree.size());
}

TEST(LeftistTree, Push)
{
    LeftistTree<int> tree;
    tree.push(1);
    EXPECT_FALSE(tree.empty());
    EXPECT_EQ(1, tree.size());
    tree.push(2);
    EXPECT_EQ(2, tree.size());
    tree.push(3);
    EXPECT_EQ(3, tree.size());
}

TEST(LeftistTree, Pop)
{
    LeftistTree<int> tree;
    tree.push(1);
    tree.push(2);
    tree.push(3);
    EXPECT_EQ(tree.pop(), 1);
    EXPECT_EQ(2, tree.size());
    EXPECT_EQ(tree.pop(), 2);
    EXPECT_EQ(1, tree.size());
    EXPECT_EQ(tree.pop(), 3);
    EXPECT_EQ(0, tree.size());
}

TEST(LeftistTree, Assign)
{
    std::vector<int> vec{1, 2, 3, 4, 5, -1, -2, -3, -4, -5};

    LeftistTree<int> tree;
    tree.assign(vec.begin(), vec.end());
    EXPECT_EQ(vec.size(), tree.size());
    EXPECT_EQ(-5, tree.pop());
    EXPECT_EQ(-4, tree.pop());
}

TEST(LeftistTree, Clear)
{
    LeftistTree<int> tree;
    tree.push(1);
    tree.push(2);
    tree.push(3);
    tree.clear();
    EXPECT_TRUE(tree.empty());
    EXPECT_EQ(0, tree.size());
}

TEST(LeftistTree, Init)
{
    LeftistTree<int> tree{1, 2, 3, 4, 5};
    EXPECT_EQ(5, tree.size());
    EXPECT_EQ(1, tree.pop());
    EXPECT_EQ(2, tree.pop());
    EXPECT_EQ(3, tree.pop());
    EXPECT_EQ(4, tree.pop());
    EXPECT_EQ(5, tree.pop());
}
