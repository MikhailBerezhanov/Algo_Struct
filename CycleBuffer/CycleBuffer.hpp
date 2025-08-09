#pragma once

#include <iterator>
#include <vector>
#include <stdexcept>

namespace AlgoStruct
{

template <typename T>
class CycleBuffer
{
public:
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;

        Iterator(CycleBuffer<T>* container, T* elemPtr)
            : m_container(container)
            , m_elemPtr(elemPtr) 
        {}

        // Operations needed for any iterator category
        Iterator() = default;
        Iterator(const Iterator& other) = default;
        Iterator& operator= (const Iterator& other) = default;
        ~Iterator() = default;
        T& operator* () { return *m_elemPtr; }

        Iterator& operator++ ()     // prefix increment: ++it;
        {
            increment(m_elemPtr);
            return *this;
        }

        Iterator operator++ (int)   // postfix increment: it++;
        {
            Iterator ret = *this;
            ++(*this);
            return ret;
        }

        // Operations needed for InputIterator
        T* operator-> () { return m_elemPtr; }
        friend bool operator== (const Iterator& lhs, const Iterator& rhs) 
        { 
            return lhs.m_container == rhs.m_container && lhs.m_elemPtr == rhs.m_elemPtr; 
        }
        friend bool operator!= (const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }

        // Operations needed for BidirectionalIterator
        Iterator& operator-- ()     // prefix decrement: --it;
        {
            decrement(m_elemPtr);
            return *this;
        }

        Iterator operator-- (int)   // postfix decrement: it--;
        {
            Iterator ret = *this;
            --(*this);
            return ret;
        }

    private:
        void increment(T*& elemPtr)
        {
            if (!elemPtr) {
                return;
            }

            const int lastIdx = (m_container->m_headIndex + m_container->m_size - 1) % m_container->m_buf.size();
            const auto* lastElemPtr = &(m_container->m_buf[lastIdx]);
            if (elemPtr == lastElemPtr) {
                elemPtr = nullptr;  // mark as end()
                return;
            }

            auto* frontElemPtr = &(m_container->m_buf.front());
            const auto* backElemPtr = &(m_container->m_buf.back());
            if (elemPtr == backElemPtr) {
                elemPtr = frontElemPtr;
            }
            else {
                ++elemPtr;
            }
        }

        void decrement(T*& elemPtr)
        {
            if (!elemPtr) {
                const int lastIdx = (m_container->m_headIndex + m_container->m_size - 1) % m_container->m_buf.size();
                elemPtr = &(m_container->m_buf[lastIdx]);
                return;
            }

            const auto* frontElemPtr = &(m_container->m_buf.front());
            auto* backElemPtr = &(m_container->m_buf.back());
            if (elemPtr == frontElemPtr) {
                elemPtr = backElemPtr;
            }
            else {
                --elemPtr;
            }
        }

    private:
        CycleBuffer<T>* m_container = nullptr;
        T* m_elemPtr = nullptr;
    };

    using iterator = Iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

    explicit CycleBuffer(int capacity)
    {
        if (capacity <= 0) {
            throw std::invalid_argument("expected capacity > 0");
        }

        m_buf.resize(capacity);
        m_buf.shrink_to_fit();
    }

    void push_back(T val);
    void push_front(T val);

    T front() const;
    T back() const;

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    iterator begin() noexcept { return m_size > 0 ? Iterator(this, &m_buf[m_headIndex]) : end(); }
    iterator end() noexcept { return Iterator(this, nullptr); }

    // std::reverse_iterator<> adapter adds decrement before dereferencing:
    // operator*() const
    // {
    //     _Iterator __tmp = current;
    //     return *--__tmp;
    // }
    //
    // So we can safely return end() as rbegin(), as it will be decremented before dereferencing
    // , rend() is not guaranteed to be dereferenceable.
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

private:
    friend Iterator;

    void increment_index(int& i) const { i = (i + 1) % m_buf.size(); }
    void decrement_index(int& i) const { i = (i - 1 + m_buf.size()) % m_buf.size(); }

private:
    size_t m_size = 0;
    std::vector<T> m_buf;
    int m_tailIndex = -1;
    int m_headIndex = 0;
};

template <typename T>
void CycleBuffer<T>::push_back(T val)
{
    increment_index(m_tailIndex);
    m_buf[m_tailIndex] = val;

    if (m_size == m_buf.size()) {
        increment_index(m_headIndex);
    }
    else {
        ++m_size;
    }
}

template <typename T>
void CycleBuffer<T>::push_front(T val)
{
    decrement_index(m_headIndex);
    m_buf[m_headIndex] = val;

    if (m_size == m_buf.size()) {
        decrement_index(m_tailIndex);
    }
    else {
        ++m_size;
    }
}

template <typename T>
T CycleBuffer<T>::back() const
{
    if (empty()) {
        throw std::runtime_error("back() on empty buffer");
    }

    return m_buf[m_tailIndex];
}

template <typename T>
T CycleBuffer<T>::front() const
{
    if (empty()) {
        throw std::runtime_error("front() on empty buffer");
    }

    return m_buf[m_headIndex];
}

} // namespace Also_Struct


/*
--- size: 1

buf        |      data_repr

t h
 [0]               ()


a) push_back(1)

   ht
  [1]               (1)

b) push_front(-1)

  t h
   [-1]             (-1)

c) push_back(1), push_front(-2)

   th
  [1]

   ht
  [-2]

--- size: 2

t  h  
  [0, 0]         ()

a) push_back(1) , push_back(2)

   ht  
  [1, 0]         (1)         

   h  t     
  [1, 2]         (1, 2)

b) push_front(-1) , push_front(-2)                       

t      h
  [0, -1]        (-1)        

t   h    
  [-2, -1]       (-2, -1)

c) push_back(1) , push_front(-1)                        

   ht  
  [1, 0]        (1)

   t   h
  [1, -1]       (-1, 1)


d) push_front(-2) , push_back(2)                     

t      h
  [0, -2]       (-2)

   t   h
  [2, -2]       (-2, 2)
--- size: 3

t h   
 [0 0 0]

size < maxSize:
 push_back: ++t, insert

 h   t
[1 2 3]

 push_front: --h, insert

t    h
 [0 -2 -1]

push_back: ++t , insert, ++h
push_front --h, insert, --t

              head  tail                data_repr: [h++ size time)
                 v   v        t h
push_back(4) -> [1 2 3]   =  [4 2 3]  ,  (2 3 4)

                 t h           t h
push_back(5) -> [4 2 3]  =  [4 5 3]  ,   (3 4 5)

                   t h       h   t
push_back(6) -> [4 5 3]  =  [4 5 6] ,    (4 5 6)

                   h   t         t  h
push_front(-1) -> [4 5 6]  =  [4 5 -1]  ,   (-1 4 5)

                     t  h       t  h
push_front(-2) -> [4 5 -1]  =  [4 -2 -1]  ,   (-2 -1 4)

                   t  h         h      t
push_front(-3) -> [4 -2 -1]  = [-3 -2 -1]   , (-3 -2 -1)

*/