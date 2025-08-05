#pragma once

#include <initializer_list>
#include <iterator>
#include <stdexcept>

namespace detail
{
template<typename T>
struct NonConstTraits 
{
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using difference_type = std::ptrdiff_t;
};

template<typename T>
struct ConstTraits 
{
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;
    using difference_type = std::ptrdiff_t;
};

template<typename T>
struct ListNode
{
    T val;
    ListNode* next = nullptr;
    ListNode* prev = nullptr;
};

template<typename T, typename Traits>
struct Iterator
{
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = typename Traits::value_type;
    using pointer = typename Traits::pointer;
    using reference = typename Traits::reference;
    using difference_type = typename Traits::difference_type;

    Iterator() = default;
    explicit Iterator(ListNode<T>* node): m_node(node) {}
    Iterator(const Iterator&) = default;
    Iterator& operator= (const Iterator&) = default;

    reference operator* () const { return m_node->val; }
    pointer operator-> () const { return m_node; }

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
    ListNode<T>* m_node = nullptr;
};

} // namespace detail

namespace AlgoStruct
{

template <typename T>
class DoublyLinkedList
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = Iterator<T, detail::NonConstTraits<T>>;
    using const_iterator = Iterator<T, detail::ConstTraits<T>>;
    using reverse_terator = std::reverse_iterator<iterator>;
    using const_reverse_terator = std::reverse_iterator<const_iterator>;

    DoublyLinkedList() = default;
    explicit DoublyLinkedList(std::initializer_list<T> init);
    ~DoublyLinkedList();

    // Element access
    reference front() const;
    reference back() const;

    // Capacity
    size_t size() const;
    bool empty() const;

    // Modifiers
    void clear();
    void insert(iterator pos, const T& value);
    void erase(iterator pos);

    void push_back();
    void pop_back();
    void push_front();
    void pop_front();

    // swap()
    // reverse()
    // merge()
    // sort()

private:
    // friend Iterator;

};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(std::initializer_list<T> il)
{

}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    // clear();
}

} // namespace AlgoStruct