#include "ForwardList.h"

#include <stdexcept>

namespace AlgoStruct
{
    void ForwardList::push_front(int value)
    {
        Node *new_node = new Node{value, nullptr};

        if (!m_head)
        {
            m_head = new_node;
            return;
        }

        new_node->next = m_head;
        m_head = new_node;
    }

    void ForwardList::pop_front()
    {
        if (!m_head)
        {
            return;
        }

        Node *to_delete = m_head;
        m_head = m_head->next;

        delete(to_delete);
    }

    ForwardList::Node* ForwardList::find_node(int value) const
    {
        Node *curr_node = m_head;

        while (curr_node)
        {
            if (curr_node->value == value)
            {
                return curr_node;
            }
            curr_node = curr_node->next;
        }

        return nullptr;
    }

    void ForwardList::insert_after(Node *node, int value)
    {
        if (!node)
        {
            return;
        }

        Node *new_node = new Node{value, node->next};
        node->next = new_node;
    }

    // reverses the order of the elements
    void ForwardList::reverse()
    {
        Node *curr_node = m_head;
        Node *prev_node = nullptr;

        while (curr_node)
        {
            Node *node = curr_node;
            curr_node = curr_node->next;

            node->next = prev_node;
            prev_node = node;
        }

        m_head = prev_node;
    }

    // erases an element after an element
    ForwardList::Node* ForwardList::erase_after(Node *node)
    {
        if (!node || !node->next)
        {
            return nullptr;
        }

        Node *to_delete = node->next;
        node->next = node->next->next;
        delete(to_delete);

        return node->next;
    }

    // clears the contents
    void ForwardList::clear()
    {
        Node *curr_node = m_head;

        while (curr_node)
        {
            Node *to_delete = curr_node;
            curr_node = curr_node->next;
            delete(to_delete);
        }

        m_head = nullptr;
    }

    int& ForwardList::front()
    {
        if (!m_head)
        {
            throw std::runtime_error("front() on empty list");
        }

        return m_head->value;
    }




    // Friend functions

    std::vector<int> traverse(const ForwardList& list)
    {
        std::vector<int> values;

        auto curr_node = list.m_head;

        while (curr_node)
        {
            values.push_back(curr_node->value);
            curr_node = curr_node->next;
        }

        return values;
    }
} // namespace AlgoStruct