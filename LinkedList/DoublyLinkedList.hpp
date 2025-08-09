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
        ListNode(T v = T{}, ListNode* n = nullptr, ListNode* p = nullptr)
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

    DoublyLinkedList(): m_beforeHead(new ListNode()) {}
    explicit DoublyLinkedList(std::initializer_list<T> init);
    ~DoublyLinkedList();

    // Element access
    T& front() const;
    T& back() const;

    // Iterators
    iterator begin() const noexcept { return empty() ? end() : iterator(m_beforeHead->next); }
    iterator end() const noexcept { return iterator(m_beforeHead); }
    iterator rbegin() const noexcept { return end(); }
    iterator rend() const noexcept { return begin(); }

    // Capacity
    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    // Modifiers
    void clear();
    iterator insert(iterator pos, const T& value);  // Inserts a copy of value before pos
    void erase(iterator pos);                       // Removes the element at pos

    void push_back(const T& val) { insert(end(), val); }
    void pop_back();
    void push_front(const T& val) { insert(begin(), val); }
    void pop_front();

    // swap()
    void reverse();
    // merge()
    // sort()

private:
    /* 
    Managing dummy node, making list cycled underhood:
       m_beforeHead->next - head
       m_beforeHead->prev - tail
    
                    [head]               [tail]
                       v                   v
       beforeHead -> node1 <-> node2 <-> node3
             ^____________________________/

    */
    ListNode* m_beforeHead = nullptr;
    size_t m_size = 0;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(std::initializer_list<T> init)
    : DoublyLinkedList()
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
    if (!m_beforeHead->next) throw std::invalid_argument("front() on empty DoublyLinkedList");

    return m_beforeHead->next->val;
}

template <typename T>
T& DoublyLinkedList<T>::back() const
{
    if (!m_beforeHead->prev) throw std::invalid_argument("back() on empty DoublyLinkedList");

    return m_beforeHead->prev->val;
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
    ListNode* nodeToDelete = nullptr;
    ListNode* currNode = m_beforeHead->next;

    while (currNode && currNode != m_beforeHead)
    {
        nodeToDelete = currNode;
        currNode = currNode->next;
        delete(nodeToDelete);
    }

    m_beforeHead->next = nullptr;
    m_beforeHead->prev = nullptr;
    m_size = 0;
}

template <typename T>
auto DoublyLinkedList<T>::insert(iterator pos, const T& value) -> iterator
{
    auto newNode = new ListNode(value);

    if (pos == end())
    {
        // Insertion at the tail
        if (m_beforeHead->prev)
        {
            m_beforeHead->prev->next = newNode;
            newNode->prev = m_beforeHead->prev;
        }
        else
        {
           m_beforeHead->next = newNode;
           newNode->prev = m_beforeHead;
        }

        // Add [end marker]: cycle list for iterators equality checks
        newNode->next = m_beforeHead;
        m_beforeHead->prev = newNode;
    }
    else if (pos == begin())
    {
        // Insertion before head
        newNode->next = m_beforeHead->next;
        newNode->prev = m_beforeHead;
        if (m_beforeHead->next) m_beforeHead->next->prev = newNode;
        m_beforeHead->next = newNode;
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
    return iterator(newNode);
}

template <typename T>
void DoublyLinkedList<T>::erase(iterator pos)
{
// TODO:
}

// External operations
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