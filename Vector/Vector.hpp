#pragma once

#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <cstring>

namespace AlgoStruct 
{
template<typename T>
class Vector
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
        reference operator* () const { return *m_bufPtr; }
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
        Iterator& operator+= (difference_type n) 
        { 
            m_bufPtr += n;
            return *this;
        }
        friend Iterator operator+ (Iterator it, difference_type n) { return it += n; }
        friend Iterator operator+ (difference_type n, Iterator it) { return it + n; }

        Iterator& operator-= (difference_type n)
        {
            m_bufPtr -= n;
            return *this;
        }
        friend Iterator operator- (Iterator it, difference_type n) { return it -= n; }
        friend difference_type operator- (const Iterator& lhs, const Iterator& rhs) { return lhs.m_bufPtr - rhs.m_bufPtr; }

        reference operator[] (difference_type n) const { return *(m_bufPtr + n); }
        friend bool operator< (const Iterator& lhs, const Iterator& rhs) { return lhs.m_bufPtr < rhs.m_bufPtr; }
        friend bool operator> (const Iterator& lhs, const Iterator& rhs) { return rhs < lhs; }
        friend bool operator<= (const Iterator& lhs, const Iterator& rhs) { return !(lhs > rhs); }
        friend bool operator>= (const Iterator& lhs, const Iterator& rhs) { return !(lhs < rhs); }

    private:
        T* m_bufPtr = nullptr;
    };

    using iterator = Iterator;
    using reverse_iterator = std::reverse_iterator<iterator>;

    Vector() = default;
    explicit Vector(std::initializer_list<T> init);
    Vector(size_t size, T initialVal = T{});
    ~Vector();

    // TODO: deep-copy semantics
    Vector(const Vector& other);  
    Vector& operator= (const Vector& other);
    // Movement semantics
    Vector(Vector&& other) noexcept;
    Vector& operator= (Vector&& other) noexcept;

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
    void reserve(size_t capacity);

    // Modifiers
    void clear();
    void push_back(const T& val);
    void pop_back();
    void resize(size_t size , T initialVal = T{});
    void swap(Vector& other) noexcept;

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
Vector<T>::Vector(std::initializer_list<T> init)
{
    this->reserve(init.size());
    for (const auto& elem : init)
    {
        this->push_back(elem);
    }
}

template<typename T>
Vector<T>::Vector(size_t size, T initialVal)
{
    this->resize(size, initialVal);
}

template<typename T>
Vector<T>::Vector(const Vector& other)
{

}

template<typename T>
Vector<T>::Vector(Vector&& other) noexcept
{
    Vector tmp;
    this->swap(other);
    other.swap(tmp);
}

template<typename T>
Vector<T>::~Vector()
{
    this->clear();
}

template<typename T>
Vector<T>& Vector<T>::operator= (const Vector<T>& other)
{
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator= (Vector<T>&& other) noexcept
{
    Vector tmp;
    this->swap(other);
    other.swap(tmp);
    return *this;
}

template<typename T>
T& Vector<T>::front() const
{
    if (empty()) throw std::invalid_argument("front() on empty Vector");
    return *m_buf;
}

template<typename T>
T& Vector<T>::back() const
{
    if (empty()) throw std::invalid_argument("back() on empty Vector");
    return *(m_buf + m_size - 1);
}

template<typename T>
void Vector<T>::reserve(size_t capacity)
{
    if (capacity <= m_capacity) return;

    reallocate_buffer(capacity);
}

template<typename T>
void Vector<T>::clear()
{
    delete[] m_buf;
    m_size = 0;
    m_capacity = 0;
}

template<typename T>
void Vector<T>::reallocate_buffer(size_t newCapacity)
{
    // Reallocate internal buffer
    m_capacity = newCapacity;
    auto newBuf = new T[m_capacity];

    // Copy existing elements
    std::memmove(newBuf, m_buf, sizeof(T) * m_size);
    std::swap(m_buf, newBuf);

    // Deallocate old storage
    delete[] newBuf;
}

template<typename T>
void Vector<T>::push_back(const T& val)
{
    if (m_size == m_capacity)
    {
        // Extend capacity
        const auto newCapacity = (m_capacity + 1) * CapExtensionFactor;
        reallocate_buffer(newCapacity);    
    }

    m_buf[m_size++] = val;
}

template<typename T>
void Vector<T>::pop_back()
{
    if (empty()) return;

    --m_size;
}

template<typename T>
void Vector<T>::resize(size_t size, T initialVal)
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
        // Virtually erase extra elements
        m_size = size;
    }
}

template<typename T>
void Vector<T>::swap(Vector& other) noexcept
{
    std::swap(m_buf, other.m_buf);
    std::swap(m_size, other.m_size);
    std::swap(m_capacity, other.m_capacity);
}

} // namespace AlgoStruct