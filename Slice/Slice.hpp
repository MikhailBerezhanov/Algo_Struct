#pragma once

#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <cstring>

namespace AlgoStruct 
{
template<typename T>
class Slice
{
public:
    class Iterator
    {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;

        // Base iterator operations
        Iterator() = default;
        Iterator(T* bufPtr): m_bufPtr(bufPtr) {}
        T& operator* () const { return *m_bufPtr; }
        Iterator& operator++ ()
        {
            ++m_bufPtr;
            return *this;
        }
        Iterator operator++ (int)
        {
            auto ret = *this;
            ++(*this);
            return ret;
        }

        // Input iterator operations
        T* operator-> () const { return *m_bufPtr; }
        friend bool operator== (const Iterator& lhs, const Iterator& rhs) { return lhs.m_bufPtr == rhs.m_bufPtr; }
        friend bool operator!= (const Iterator& lhs, const Iterator& rhs) { return !(lhs == rhs); }

        // Bidirectional iterator operations
        Iterator& operator-- ()
        {
            --m_bufPtr;
            return *this;
        }
        Iterator operator-- (int)
        {
            auto ret = *this;
            --(*this);
            return ret;
        }

        // Random access iterator operations
        // operator+=
        // operator-=
        // operator>
        // operator<=
        // operator<
        // operator>=

    private:
        T* m_bufPtr = nullptr;
    };

    using iterator = Iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

    Slice() = default;
    explicit Slice(std::initializer_list<T> init);
    Slice(size_t size, T initialVal = T{});
    ~Slice();

    // TODO: deep-copy
    Slice(const Slice& other);  
    Slice& operator= (const Slice& other);
    // TODO: move
    Slice(Slice&& other);
    Slice& operator= (Slice&& other);

    // Element access
    T& front() const;
    T& back() const;
    T& operator[] (size_t idx) { return *(m_buf + idx); }

    // Iterators
    iterator begin() const noexcept { return iterator(m_buf); }
    iterator end() const noexcept { return iterator(m_buf + m_size); }
    // reverse_terator rbegin() const noexcept { return reverse_terator(end()); }
    // reverse_terator rend() const noexcept { return reverse_terator(begin()); }

    // Capacity
    size_t size() const { return m_size; }
    size_t capacity() const { return m_capacity; }
    bool empty() const { return m_size == 0; }
    void resize(size_t size , T initialVal = T{});
    void reserve(size_t capacity);

    // Modifiers
    void clear();
    void push_back(const T& val);
    void pop_back();

private:
    void reallocate_buffer(size_t newCapacity);

private:
    friend iterator;
    T* m_buf = nullptr;
    size_t m_size = 0;
    size_t m_capacity = 0;
    static constexpr int CapExtensionFactor = 2;
};

template<typename T>
Slice<T>::Slice(std::initializer_list<T> init)
{
    this->reserve(init.size());
    for (const auto& elem : init)
    {
        this->push_back(elem);
    }
}

template<typename T>
Slice<T>::Slice(size_t size, T initialVal)
{
    this->resize(size, initialVal);
}

template<typename T>
Slice<T>::~Slice()
{
    this->clear();
}

template<typename T>
T& Slice<T>::front() const
{
    if (empty()) throw std::invalid_argument("front() on empty Slice");
    return *m_buf;
}

template<typename T>
T& Slice<T>::back() const
{
    if (empty()) throw std::invalid_argument("back() on empty Slice");
    return *(m_buf + m_size - 1);
}

template<typename T>
void Slice<T>::resize(size_t size, T initialVal)
{
    if (size > m_size)
    {
        for (auto i = 0; i < size - m_size; ++i)
        {
            this->push_back(initialVal);
        }
    }
    else if (size < m_size)
    {
        // Erase extra elements
        m_size = size;
    }
}

template<typename T>
void Slice<T>::reserve(size_t capacity)
{
    if (capacity <= m_capacity) return;

    reallocate_buffer(capacity);
}

template<typename T>
void Slice<T>::clear()
{
    delete[] m_buf;
    m_size = 0;
    m_capacity = 0;
}

template<typename T>
void Slice<T>::reallocate_buffer(size_t newCapacity)
{
    printf("reallocate_buffer, newCapacity: %ld\n", newCapacity);
    // Reallocate internal buffer
    m_capacity = newCapacity;
    auto newBuf = new T[m_capacity];

    // Copy existing data
    std::memmove(newBuf, m_buf, sizeof(T) * m_size);
    std::swap(m_buf, newBuf);

    // Deallocate old storage
    delete[] newBuf;
}

template<typename T>
void Slice<T>::push_back(const T& val)
{
    if (m_size == m_capacity)
    {
        // Extend capacity
        const auto newCapacity = (m_capacity + 1) * CapExtensionFactor;
        reallocate_buffer(newCapacity);    
    }

    m_buf[m_size++] = val;
}

} // anmespace AlgoStruct