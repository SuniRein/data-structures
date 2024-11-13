#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>

template<typename T>
class BinomialHeap
{
  public:
    using ValueType = T;
    using SizeType  = std::ptrdiff_t;

    BinomialHeap() = default;

    BinomialHeap(BinomialHeap const&)            = delete;
    BinomialHeap& operator=(BinomialHeap const&) = delete;

    BinomialHeap(std::initializer_list<T> list)
    {
        for (auto const& key : list)
        {
            push(key);
        }
    }

    void      push(ValueType key);
    ValueType pop();
    void      merge(BinomialHeap& other);

    template<typename InputIt>
    void assign(InputIt first, InputIt last)
    {
        clear();
        while (first != last)
        {
            push(*first++);
        }
    }

    [[nodiscard]]
    ValueType const& top() const
    {
        return min_->key;
    }

    [[nodiscard]]
    bool empty() const
    {
        return !root_;
    }

    [[nodiscard]]
    SizeType size() const
    {
        return size_;
    }

    void clear();

  private:
    struct Node
    {
        ValueType key{};
        SizeType  degree{};
        Node*     child{};
        Node*     sibling{};

        explicit Node(ValueType key): key{std::move(key)} {}

        ~Node() noexcept
        {
            delete child;
            delete sibling;
        }
    };

    Node*    root_{};
    Node*    min_{};
    SizeType size_{};

    // 连接两个二项堆，成为一个节点度数递增的二项树链表
    static Node* link_heap(Node* lhs, Node* rhs);

    // 连接度数相同的两棵二项树，使其中一棵成为另一棵的子树
    static void link_tree(Node* child, Node* root);

    // 合并两个二项堆
    static Node* merge(Node* lhs, Node* rhs);

    // 反转二项树链表
    static Node* reverse(Node* head)
    {
        Node* prev{};
        Node* curr{head};
        Node* next{};
        while (curr)
        {
            next          = curr->sibling;
            curr->sibling = prev;
            prev          = curr;
            curr          = next;
        }
        return prev;
    }
};

template<typename T>
auto BinomialHeap<T>::link_heap(Node* lhs, Node* rhs) -> Node*
{
    if (!lhs)
    {
        return rhs;
    }
    if (!rhs)
    {
        return lhs;
    }

    Node* head{};
    Node* tail{};

    // 选取度数最小的节点作为第一项
    if (lhs->degree < rhs->degree)
    {
        head = lhs;
        lhs  = lhs->sibling;
    }
    else
    {
        head = rhs;
        rhs  = rhs->sibling;
    }
    tail = head;

    while (lhs && rhs)
    {
        if (lhs->degree < rhs->degree)
        {
            tail->sibling = lhs;
            lhs           = lhs->sibling;
        }
        else
        {
            tail->sibling = rhs;
            rhs           = rhs->sibling;
        }
        tail = tail->sibling;
    }
    tail->sibling = lhs ? lhs : rhs;
    return head;
}

template<typename T>
void BinomialHeap<T>::link_tree(Node* child, Node* root)
{
    child->sibling = root->child;
    root->child    = child;
    ++root->degree;
}

template<typename T>
auto BinomialHeap<T>::merge(Node* lhs, Node* rhs) -> Node*
{
    Node* root = link_heap(lhs, rhs);
    if (!root)
    {
        return nullptr;
    }

    Node* prev{};
    Node* curr{root};
    Node* next{curr->sibling};

    while (next)
    {
        // 情况1：当前节点的度数不等于下一个节点的度数
        // 情况2：当前节点的度数等于下一个节点的度数，且下一个节点的度数等于下一个节点的下一个节点的度数
        // 以上两种情况，保留当前节点，移动指针
        if (curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree))
        {
            prev = curr;
            curr = next;
        }
        // 情况3：当前节点的度数等于下一个节点的度数，且当前节点的关键字小于下一个节点的关键字
        // 保留当前节点，将下一个节点链接到当前节点的子树中
        else if (curr->key <= next->key)
        {
            curr->sibling = next->sibling;
            link_tree(next, curr);
        }
        // 情况4：当前节点的度数等于下一个节点的度数，且当前节点的关键字大于下一个节点的关键字
        // 保留下一个节点，将当前节点链接到下一个节点的子树中
        else
        {
            if (!prev)
            {
                root = next;
            }
            else
            {
                prev->sibling = next;
            }
            link_tree(curr, next);
            curr = next;
        }
        next = curr->sibling;
    }

    return root;
}

template<typename T>
auto BinomialHeap<T>::pop() -> ValueType
{
    ValueType min_key = std::move(min_->key);
    --size_;

    // 找到最小节点及其前驱
    Node* prev{};
    Node* curr{root_};
    while (curr != min_)
    {
        prev = curr;
        curr = curr->sibling;
    }

    // 从二叉堆中删除最小节点的子树
    if (!prev)
    {
        root_ = curr->sibling;
    }
    else
    {
        prev->sibling = curr->sibling;
    }

    // 遍历两个新的二叉堆，维护最小节点
    min_       = nullptr;
    Node* head = root_;
    while (head)
    {
        if (!min_ || head->key < min_->key)
        {
            min_ = head;
        }
        head = head->sibling;
    }
    Node* min_head = curr->child;
    while (min_head)
    {
        if (!min_ || min_head->key < min_->key)
        {
            min_ = min_head;
        }
        min_head = min_head->sibling;
    }

    // 反转最小节点的子树
    curr->child = reverse(curr->child);

    // 合并两个二叉堆
    root_ = merge(root_, curr->child);

    // 释放最小节点
    curr->child   = nullptr;
    curr->sibling = nullptr;
    delete curr;

    return min_key;
}

template<typename T>
void BinomialHeap<T>::clear()
{
    delete root_;
    root_ = nullptr;
    min_  = nullptr;
    size_ = 0;
}

template<typename T>
void BinomialHeap<T>::push(ValueType key)
{
    Node* node = new Node{std::move(key)};
    root_      = merge(root_, node);
    if (!min_ || node->key < min_->key)
    {
        min_ = node;
    }
    ++size_;
}

template<typename T>
void BinomialHeap<T>::merge(BinomialHeap& other)
{
    root_ = merge(root_, other.root_);
    if (!min_ || (other.min_ && other.min_->key < min_->key))
    {
        min_ = other.min_;
    }
    size_       += other.size_;
    other.root_  = nullptr;
    other.min_   = nullptr;
    other.size_  = 0;
}
