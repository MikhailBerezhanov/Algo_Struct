#pragma once
#include <vector>

namespace AlgoStruct
{
    class ForwardList
    {
        friend std::vector<int> traverse(const ForwardList& list);

    public:
        struct Node
        {
            Node(int v = 0, Node *n = nullptr): value(v), next(n) {}
            int value = 0;
            Node *next = nullptr;
        };

        ForwardList() = default;
        ~ForwardList()
        {
            this->clear();
        }

        // inserts an element to the beginning
        void push_front(int value);

        void pop_front();

        bool empty() const
        {
            return m_head == nullptr;
        }

        // returns first node with desired value
        Node* find_node(int value) const;

        // inserts elements after an element
        void insert_after(Node *node, int value);

        // reverses the order of the elements
        void reverse();

        // removes an element after desired element
        Node* erase_after(Node *node);

        // clears the contents of the list
        void clear();

        int& front();

        const int& front() const
        {
            return const_cast<ForwardList*>(this)->front();
        }

    private:
        Node *m_head = nullptr;
    };

    // std::vector<int> traverse(const ForwardList& list);
} // namespace AlgoStruct