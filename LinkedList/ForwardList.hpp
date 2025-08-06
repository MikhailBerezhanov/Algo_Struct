#pragma once

#include <iterator>
#include <cstddef>
#include <stdexcept>

#include <initializer_list>

namespace AlgoStruct
{
    template <typename T>
    class ForwardList
    {
        struct Node
        {
            Node(T v, Node* n = nullptr): value(v), next(n) {}
            T value;
            Node* next = nullptr;
        };

    public:
        class Iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            Iterator() = default;
            explicit Iterator(Node* node): m_node(node) {}

            Iterator& operator= (const Iterator& rhs)
            {
                m_node = rhs.m_node;
                return *this;
            }

            reference operator* () const { return m_node->value; }
            pointer operator-> () const { return m_node->value; }

            Iterator& operator++ ()
            {
                m_node = m_node->next;
                return *this;
            }

            Iterator operator++ (int)
            {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            friend bool operator== (const Iterator& lhs, const Iterator& rhs) noexcept
            {
                return lhs.m_node == rhs.m_node;
            }

            friend bool operator!= (const Iterator& lhs, const Iterator& rhs) noexcept
            {
                return !(lhs == rhs);
            }

            operator bool () const
            {
                return m_node != nullptr;
            }

            Iterator& operator+= (difference_type n)
            {
                while (n--)
                {
                    m_node = m_node->next;
                }

                return *this;
            }

        private:
            friend ForwardList<T>;
            Node* m_node = nullptr;
        };

        ForwardList() = default;
        ForwardList(std::initializer_list<T> il);
        ~ForwardList();

        // Iterators getters
        Iterator begin() const noexcept;
        Iterator end() const noexcept;

        // Size checkers 
        bool empty() const noexcept;
        size_t size() const noexcept;

        // Inserts an element to the list
        void push_front(const T& value);
        void push_back(const T& value);

        // Removes a head element from the list
        void pop_front();

        // Get access to head element value
        T& front();
        const T& front() const;

        // Get access to tail alement value
        T& back();
        const T& back() const;

        // Inserts elements after an desired place
        void insert_after(Iterator& it, const T& value);

        // Removes an element after desired element
        Iterator erase_after(Iterator& it);

        // Reverses the order of the elements
        void reverse();

        // Clears the contents of the list
        void clear();

        // Merge sort of list elements
        void sort();

    private:
        Node* split(Node* head);
        std::pair<Node*, Node*> merge(Node* left_head, Node* right_head);
        std::pair<Node*, Node*> merge_sort(Node* head);

    private:
        Node* m_head = nullptr;
        Node* m_tail = nullptr;
        size_t m_size = 0;
    };

    template <typename T>
    ForwardList<T>::ForwardList(std::initializer_list<T> il)
    {
        for (const auto& elem : il)
        {
            push_back(elem);
        }
    }

    template <typename T>
    ForwardList<T>::~ForwardList()
    {
        clear();
    }

    template <typename T>
    auto ForwardList<T>::begin() const noexcept -> Iterator
    {
        return Iterator(m_head);
    }

    template <typename T>
    auto ForwardList<T>::end() const noexcept -> Iterator
    {
        return Iterator(m_tail->next);
    }

    template <typename T>
    bool ForwardList<T>::empty() const noexcept
    {
        return m_head == nullptr;
    }

    template <typename T>
    size_t ForwardList<T>::size() const noexcept
    {
        return m_size;
    }

    template <typename T>
    void ForwardList<T>::push_front(const T& value)
    {
        Node* new_node = new Node{value, nullptr};

        if (!m_head)
        {
            m_head = new_node;
            m_tail = m_head;
        }
        else
        {
            new_node->next = m_head;
            m_head = new_node;
        }

        ++m_size;
    }

    template <typename T>
    void ForwardList<T>::push_back(const T& value)
    {
        Node* new_node = new Node{value, nullptr};

        if (!m_tail)
        {
            m_head = new_node;
            m_tail = m_head;
        }
        else
        {
            m_tail->next = new_node;
            m_tail = new_node;
        }
        
        ++m_size;
    }

    template <typename T>
    void ForwardList<T>::pop_front()
    {
        if (!m_head)
        {
            return;
        }

        Node* to_delete = m_head;
        m_head = m_head->next;

        delete(to_delete);

        if (!m_head)
        {
            m_tail = nullptr;
        }

        --m_size;
    }

    template <typename T>
    void ForwardList<T>::insert_after(Iterator& it, const T& value)
    {
        if (!it)
        {
            return;
        }

        Node *new_node = new Node{value, it.m_node->next};
        it.m_node->next = new_node;

        if (it == Iterator(m_tail))
        {
            m_tail = new_node;
        }

        ++m_size;
    }

    template <typename T>
    auto ForwardList<T>::erase_after(Iterator& it) -> Iterator
    {
        if (!it || !it.m_node->next)
        {
            return end();
        }

        Node* to_delete = it.m_node->next;
        it.m_node->next = it.m_node->next->next;
        delete(to_delete);

        if (to_delete == m_tail)
        {
            m_tail = it.m_node;
        }

        --m_size;
        return Iterator{it.m_node->next};
    }

    template <typename T>
    void ForwardList<T>::reverse()
    {
        m_tail = m_head;
        Node *curr_node = m_head;
        Node *prev_node = nullptr;

        while (curr_node)
        {
            Node* node = curr_node;
            curr_node = curr_node->next;

            node->next = prev_node;
            prev_node = node;
        }

        m_head = prev_node;
    }

    template <typename T>
    void ForwardList<T>::clear()
    {
        Node* curr_node = m_head;

        while (curr_node)
        {
            Node* to_delete = curr_node;
            curr_node = curr_node->next;
            delete(to_delete);
        }

        m_head = nullptr;
        m_tail = nullptr;
        m_size = 0;
    }

    template <typename T>
    T& ForwardList<T>::front()
    {
        if (!m_head)
        {
            throw std::runtime_error("front() on empty list");
        }

        return m_head->value;
    }

    template <typename T>
    const T& ForwardList<T>::front() const
    {
        return const_cast<ForwardList*>(this)->front();
    }

    template <typename T>
    T& ForwardList<T>::back()
    {
        if (!m_tail)
        {
            throw std::runtime_error("back() on empty list");
        }

        return m_tail->value;
    }

    template <typename T>
    const T& ForwardList<T>::back() const
    {
        return const_cast<ForwardList*>(this)->back();
    }

    template <typename T>
    auto ForwardList<T>::split(Node* head) -> Node*
    {
        Node* slow = head;
        Node* fast = head;
        Node* mid = slow;

        while (fast && fast->next)
        {
            mid = slow;
            slow = slow->next;
            fast = fast->next->next;
        }

        return mid;
    }

    template <typename T>
    auto ForwardList<T>::merge(Node* left_head, Node* right_head) -> std::pair<Node*, Node*>
    {
        Node* new_head = nullptr;
        Node* new_tail = nullptr;

        const auto append_node = [&new_head, &new_tail](Node* node)
        {
            if (!new_head)
            {
                new_head = node;
                new_tail = new_head;
            }
            else
            {
                new_tail->next = node;
                new_tail = new_tail->next;
            }
        };

        while (left_head && right_head)
        {
            if (left_head->value <= right_head->value)
            {
                append_node(left_head);
                left_head = left_head->next;
            }
            else
            {
                append_node(right_head);
                right_head = right_head->next;
            }
        }

        while (left_head)
        {
            append_node(left_head);
            left_head = left_head->next;
        }

        while (right_head)
        {
            append_node(right_head);
            right_head = right_head->next;
        }

        return {new_head, new_tail};
    }

    template <typename T>
    auto ForwardList<T>::merge_sort(Node* head) -> std::pair<Node*, Node*>
    {
        // 3 -> 2 -> 1 -> 5 -> 4 ->
        //          split
        // 3 -> 2 ->        1 -> 5 -> 4 ->
        //   split              split
        // 3 ->     2 ->    1 ->   5 -> 4 ->
        //     merge                split
        // 2 -> 3 ->        1 ->    5 ->   4 -> 
        //                             merge
        // 2 -> 3 ->        1 ->    4 -> 5 ->
        //                     merge
        // 2 -> 3 ->        1 -> 4 -> 5 ->
        //           merge 
        // 1 -> 2 -> 3 -> 4 -> 5 ->

        if (!head || !head->next)
        {
            return {head, head};
        }

        Node* mid = split(head);
        Node* left_head = head;
        Node* right_head = mid->next;
        mid->next = nullptr;

        const auto left_sorted = merge_sort(left_head);
        const auto right_sorted = merge_sort(right_head);

        return merge(left_sorted.first, right_sorted.first);
    }

    template <typename T>
    void ForwardList<T>::sort()
    {
        const auto head_tail = merge_sort(m_head);

        m_head = head_tail.first;
        m_tail = head_tail.second;
    }

} // namespace AlgoStruct
