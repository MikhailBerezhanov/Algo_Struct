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
            friend Iterator ForwardList<T>::erase_after(Iterator& it);
            
        public:
            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = T;
            using pointer = T*;
            using reference = T&;

            Iterator() = default;
            explicit Iterator(Node* node): m_current(node) {}

            Iterator& operator= (const Iterator& rhs)
            {
                m_current = rhs.m_current;
                return *this;
            }

            Node* operator-> ()
            {
                return m_current;
            }

            int& operator* ()
            {
                return m_current->value;
            }

            Iterator& operator++ ()
            {
                m_current = m_current->next;
                return *this;
            }

            Iterator operator++ (int)
            {
                Iterator tmp = *this;
                ++*this;
                return tmp;
            }

            friend bool operator== (const Iterator& lhs, const Iterator& rhs)
            {
                return lhs.m_current == rhs.m_current;
            }

            friend bool operator!= (const Iterator& lhs, const Iterator& rhs)
            {
                return !(lhs == rhs);
            }

            operator bool () const
            {
                return m_current != nullptr;
            }

            Iterator& operator+= (difference_type n)
            {
                while (n--)
                {
                    m_current = m_current->next;
                }

                return *this;
            }

        private:
            Node* m_current = nullptr;
        };

        ForwardList() = default;
        ForwardList(std::initializer_list<T> il);
        ~ForwardList();

        // Iterators getters
        Iterator begin() const;
        Iterator end() const;

        // Size checkers 
        bool empty() const;
        size_t size() const;

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
    auto ForwardList<T>::begin() const -> Iterator
    {
        return Iterator(m_head);
    }

    template <typename T>
    auto ForwardList<T>::end() const -> Iterator
    {
        return Iterator(m_tail->next);
    }

    template <typename T>
    bool ForwardList<T>::empty() const
    {
        return m_head == nullptr;
    }

    template <typename T>
    size_t ForwardList<T>::size() const
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

        Node *new_node = new Node{value, it->next};
        it->next = new_node;

        if (it == Iterator(m_tail))
        {
            m_tail = new_node;
        }

        ++m_size;
    }

    template <typename T>
    auto ForwardList<T>::erase_after(Iterator& it) -> Iterator
    {
        if (!it || !it->next)
        {
            return end();
        }

        Node* to_delete = it->next;
        it->next = it->next->next;
        delete(to_delete);

        if (to_delete == m_tail)
        {
            m_tail = it.m_current;
        }

        --m_size;
        return Iterator{it->next};
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
            throw std::runtime_error("front() on empty list");
        }

        return m_tail->value;
    }

    template <typename T>
    const T& ForwardList<T>::back() const
    {
        return const_cast<ForwardList*>(this)->back();
    }


// Helper functions


    // bool has_cycle(const ForwardList::Node* head);

} // namespace AlgoStruct