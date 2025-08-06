#pragma once

#include <initializer_list>
#include <iterator>
#include <stdexcept>

namespace AlgoStruct
{

template <typename T>
class DoublyLinkedList
{
private:
    struct ListNode
    {
        ListNode(T v, ListNode* n = nullptr, ListNode* p = nullptr)
            : val(std::move(v)), next(n), prev(p) {}

        T val;
        ListNode* next = nullptr;
        ListNode* prev = nullptr;
    };

    struct Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;

        Iterator() = default;
        explicit Iterator(ListNode* node): m_node(node) {}
        Iterator(const Iterator&) = default;
        Iterator& operator= (const Iterator&) = default;

        reference operator* () const { return m_node->val; }
        pointer operator-> () const { return m_node->val; }

        Iterator& operator++ ()
        {
            m_node = m_node->next;
            return *this;
        }
        Iterator operator++ (int)
        {
            Iterator ret = *this;
            ++(*this);
            return ret;
        }

        friend bool operator== (const Iterator& lhs, const Iterator& rhs) { return lhs.m_node == rhs.m_node; };
        friend bool operator!= (const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }

        Iterator& operator-- ()
        {
            m_node = m_node->prev;
            return *this;
        }
        Iterator operator-- (int)
        {
            Iterator ret = *this;
            --(*this);
            return ret;
        }

    private:
        friend DoublyLinkedList<T>;
        ListNode* m_node = nullptr;
    };

public:
    using iterator = Iterator;
    using reverse_terator = std::reverse_iterator<iterator>;

    DoublyLinkedList() = default;
    explicit DoublyLinkedList(std::initializer_list<T> init);
    ~DoublyLinkedList();

    // Element access
    T& front() const;
    T& back() const;

    // Iterators
    iterator begin() const noexcept { return iterator(m_head); }
    iterator end() const noexcept { return iterator(m_tail->next); }

    // Capacity
    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    // Modifiers
    void clear();
    void insert(iterator pos, const T& value);  // Inserts a copy of value before pos
    void erase(iterator pos);                   // Removes the element at pos

    void push_back(const T& val);
    void pop_back();
    void push_front();
    void pop_front();

    // swap()
    // reverse()
    // merge()
    // sort()

private:
    ListNode* m_head = nullptr;
    ListNode* m_tail = nullptr;
    size_t m_size = 0;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(std::initializer_list<T> init)
{
    for (const auto& elem : init)
    {
        push_back(elem);
    }
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    clear();
}

template <typename T>
T& DoublyLinkedList<T>::front() const
{
    if (!m_head) throw std::invalid_argument("front() on empty DoublyLinkedList");

    return m_head->val;
}

template <typename T>
T& DoublyLinkedList<T>::back() const
{
    if (!m_tail) throw std::invalid_argument("tail() on empty DoublyLinkedList");

    return m_tail->val;
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
    ListNode* nodeToDelete = nullptr;

    while (m_head)
    {
        nodeToDelete = m_head;
        m_head = m_head->next;
        delete(nodeToDelete);
    }

    m_tail = nullptr;
    m_size = 0;
}

template <typename T>
void DoublyLinkedList<T>::insert(iterator pos, const T& value)
{
    if (pos == end()) throw std::invalid_argument("insertion before end()");    

    auto newNode = new ListNode(value);

    if (pos.m_node == m_head)
    {
        // Insertion before head
        m_head->prev = newNode;
        newNode->next = m_head;
        m_head = newNode;
    }
    else
    {
        // Insertion between other elements
        newNode->prev = pos.m_node->prev;
        newNode->next = pos.m_node;
        pos.m_node->prev->next = newNode;
        pos.m_node->prev = newNode;
    }

    ++m_size;
}

template <typename T>
void DoublyLinkedList<T>::erase(iterator pos)
{

}


template <typename T>
void DoublyLinkedList<T>::push_back(const T& val)
{
    auto newNode = new ListNode(val);

    if (m_tail)
    {
        m_tail->next = newNode;
        newNode->prev = m_tail;
        m_tail = newNode;
    }
    else
    {
        m_head = newNode;
        m_tail = newNode;
    }

    ++m_size;
}

template <typename T>
bool operator== (const DoublyLinkedList<T>& lhs, const DoublyLinkedList<T>& rhs)
{
    if (lhs.size() != rhs.size()) return false;

    for (auto it1 = lhs.begin(), it2 = rhs.begin(); it1 != lhs.end(); ++it1, ++it2)
    {
        if (*it1 != *it2) return false;
    }

    return true;
}

template <typename T>
bool operator!= (const DoublyLinkedList<T>& lhs, const DoublyLinkedList<T>& rhs)
{
    return !(lhs == rhs);
}

} // namespace AlgoStruct