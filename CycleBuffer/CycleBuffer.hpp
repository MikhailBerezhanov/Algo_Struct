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
        using interator_category = std::forward_iterator_tag;

        // TODO:
    };

    explicit CycleBuffer(int size)
    {
        if (size <= 0) {
            throw std::invalid_argument("expected size > 0");
        }

        m_buf.resize(size);
        m_buf.shrink_to_fit();
    }

    void push_back(T val);
    void push_front(T val);

    T front() const;
    T back() const;

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    // TODO: temp solution instead of begin() + end()
    std::vector<T> GetContent() const
    {
        std::vector<T> res;
        res.reserve(m_buf.size());
        int idx = m_head;
        for (int sz = 0; sz < m_size; ++sz) {
            res.push_back(m_buf[idx]);
            increment_marker(idx);
        }

        return res;
    }

private:
    void increment_marker(int& m) const { m = (m + 1) % m_buf.size(); }
    void decrement_marker(int& m) const { m = (m - 1 + m_buf.size()) % m_buf.size(); }

private:
    size_t m_size = 0;
    std::vector<T> m_buf;
    int m_tail = -1;
    int m_head = 0;
};

template <typename T>
void CycleBuffer<T>::push_back(T val)
{
    increment_marker(m_tail);
    m_buf[m_tail] = val;

    if (m_size == m_buf.size()) {
        increment_marker(m_head);
    }
    else {
        ++m_size;
    }
}

template <typename T>
void CycleBuffer<T>::push_front(T val)
{
    decrement_marker(m_head);
    m_buf[m_head] = val;

    if (m_size == m_buf.size()) {
        decrement_marker(m_tail);
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

    return m_buf[m_tail];
}

template <typename T>
T CycleBuffer<T>::front() const
{
    if (empty()) {
        throw std::runtime_error("front() on empty buffer");
    }

    return m_buf[m_head];
}


// GetRange
// {
    // i = h
    // m_size time(s) return m_buf[i++]
// }

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