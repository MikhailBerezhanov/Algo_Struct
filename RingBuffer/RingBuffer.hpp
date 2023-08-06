#pragma once

#include <iterator>
#include <cstddef>
#include <vector>

namespace detail
{
    template<typename T>
    struct NonConstTraits {
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using difference_type = std::ptrdiff_t;
    };

    template<typename T>
    struct ConstTraits {
        using value_type = T;
        using pointer = const T*;
        using reference = const T&;
        using difference_type = std::ptrdiff_t;
    };

    template<typename Buff, typename Traits>
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Traits::value_type;
        using pointer = typename Traits::pointer;
        using reference = typename Traits::reference;
        using difference_type = typename Traits::difference_type;

        Iterator() = default;
        Iterator(const Buff* buff, const pointer current)
            : m_buff(buff)
            , m_currentElemPtr(current)
        {
        }

        pointer operator-> ()
        {
            return m_currentElemPtr;
        }

        reference operator* ()
        {
            return *m_currentElemPtr;
        }

        Iterator& operator++ ()
        {
            m_buff->increment(m_currentElemPtr);
            return *this;
        }

        Iterator operator++ (int)
        {
            Iterator ret = *this;
            ++(*this);
            return ret;
        }

        Iterator& operator-- ()
        {
            m_buff->decrement(m_currentElemPtr);
            return *this;
        }

        Iterator operator-- (int)
        {
            Iterator ret = *this;
            --(*this);
            return ret;
        }

        operator bool () const
        {
            return m_currentElemPtr != nullptr;
        }

        friend bool operator== (const Iterator& lhs, const Iterator& rhs)
        {
            return lhs.m_currentElemPtr == rhs.m_currentElemPtr;
        }

        friend bool operator!= (const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        const Buff* m_buff = nullptr;
        pointer m_currentElemPtr = nullptr;
    };
} // namespace detail

namespace R2C
{
    template<typename T>
    class RingBuffer
    {
    public:
        using value_type = T;
        using size_type = std::size_t;
        using reference = value_type&;
        using const_reference = const value_type&;
        using pointer = value_type*;
        using const_pointer = const value_type*;
        using iterator = detail::Iterator<RingBuffer<T>, detail::NonConstTraits<T>>;
        using const_iterator = detail::Iterator<RingBuffer<T>, detail::ConstTraits<T>>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        RingBuffer() = default;
        explicit RingBuffer(size_type maxSize);
        RingBuffer(const RingBuffer& other) = default;
        RingBuffer(RingBuffer&& other) = default;

        RingBuffer& operator= (const RingBuffer& other) = default;
        RingBuffer& operator= (RingBuffer&& other) = default;

        friend bool operator== (const RingBuffer& lhs, const RingBuffer& rhs) noexcept
        {
            return lhs.m_maxSize == rhs.m_maxSize && lhs.m_storage == rhs.m_storage;
        }

        friend bool operator!= (const RingBuffer& lhs, const RingBuffer& rhs) noexcept
        {
            return !(lhs == rhs);
        }

        void push(const T& data);
        void push(T&& data);
        void swap(RingBuffer& other) noexcept;
        void clear();
        void reserve();
        void shrink_to_fit();

        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        size_t size() const;
        bool empty() const;
        bool full() const;

        T& front();
        const T& front() const;
        T& back();
        const T& back() const;

    private:
        template<typename Pointer>
        void increment(Pointer& ptr) const;

        template<typename Pointer>
        void decrement(Pointer& ptr) const;

    private:
        size_type m_maxSize = 1;
        size_type m_lastIdx = 0;
        std::vector<T> m_storage;
        bool m_isRotated = false;

        friend iterator;
        friend const_iterator;
    };

    template<typename T>
    RingBuffer<T>::RingBuffer(size_t maxSize)
        : m_maxSize(maxSize)
    {
        if (m_maxSize < 1)
        {
            throw std::logic_error("invalid ring-buffer max size");
        }
    }

    template<typename T>
    void RingBuffer<T>::push(const T& data)
    {
        if (m_storage.size() < m_maxSize)
        {
            m_storage.emplace_back(data);
        }
        else
        {
            m_lastIdx %= m_maxSize;
            m_storage[m_lastIdx] = data;

            m_isRotated = m_lastIdx + 1 < m_maxSize;
        }

        ++m_lastIdx;
    }

    template<typename T>
    void RingBuffer<T>::push(T&& data)
    {
        if (m_storage.size() < m_maxSize)
        {
            m_storage.emplace_back(std::move(data));
        }
        else
        {
            m_lastIdx %= m_maxSize;
            m_storage[m_lastIdx] = std::move(data);

            m_isRotated = m_lastIdx + 1 < m_maxSize;
        }

        ++m_lastIdx;
    }

    template<typename T>
    void RingBuffer<T>::swap(RingBuffer& other) noexcept
    {
        std::swap(this->m_maxSize, other.m_maxSize);
        std::swap(this->m_lastIdx, other.m_lastIdx);
        m_storage.swap(other.m_storage);
        std::swap(this->m_isRotated, other.m_isRotated);
    }

    template<typename T>
    void RingBuffer<T>::clear()
    {
        m_storage.clear();
        m_lastIdx = 0;
        m_isRotated = false;
    }

    template<typename T>
    void RingBuffer<T>::reserve()
    {
        m_storage.reserve(m_maxSize);
    }
    
    template<typename T>
    void RingBuffer<T>::shrink_to_fit()
    {
        m_storage.shrink_to_fit();
    }

    template<typename T>
    auto RingBuffer<T>::begin() noexcept -> iterator
    {
        if (m_isRotated)
        {
            return iterator(this, &m_storage[m_lastIdx]);
        }
        else
        {
            return m_storage.empty() ? iterator() : iterator(this, &m_storage[0]);
        }
    }

    template<typename T>
    auto RingBuffer<T>::end() noexcept -> iterator
    {
        return iterator(this, nullptr);
    }

    template<typename T>
    auto RingBuffer<T>::begin() const noexcept -> const_iterator
    {
        if (m_isRotated)
        {
            return const_iterator(this, &m_storage[m_lastIdx]);
        }
        else
        {
            return m_storage.empty() ? const_iterator() : const_iterator(this, &m_storage[0]);
        }
    }

    template<typename T>
    auto RingBuffer<T>::end() const noexcept -> const_iterator
    {
        return const_iterator(this, nullptr);
    }

    template<typename T>
    auto RingBuffer<T>::cbegin() const noexcept -> const_iterator
    {
        return begin();
    }

    template<typename T>
    auto RingBuffer<T>::cend() const noexcept -> const_iterator
    {
        return end();
    }

    template<typename T>
    auto RingBuffer<T>::rbegin() noexcept -> reverse_iterator
    {
        return reverse_iterator(end());
    }

    template<typename T>
    auto RingBuffer<T>::rend() noexcept -> reverse_iterator
    {
        return reverse_iterator(begin());
    }

    template<typename T>
    auto RingBuffer<T>::rbegin() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator(end());
    }

    template<typename T>
    auto RingBuffer<T>::rend() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator(begin());
    }

    template<typename T>
    auto RingBuffer<T>::crbegin() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator(end());
    }

    template<typename T>
    auto RingBuffer<T>::crend() const noexcept -> const_reverse_iterator
    {
        return const_reverse_iterator(begin());
    }

    template<typename T>
    T& RingBuffer<T>::front()
    {
        if (m_storage.empty())
        {
            throw std::logic_error("front on empty ring-buffer");
        }

        return *begin();
    }

    template<typename T>
    const T& RingBuffer<T>::front() const
    {
        return const_cast<RingBuffer*>(this)->front();
    }

    template<typename T>
    T& RingBuffer<T>::back()
    {
        if (m_storage.empty())
        {
            throw std::logic_error("back on empty ring-buffer");
        }

        return m_isRotated ? m_storage[m_lastIdx - 1] : m_storage.back();
    }

    template<typename T>
    const T& RingBuffer<T>::back() const
    {
        return const_cast<RingBuffer*>(this)->back();
    }

    template<typename T>
    size_t RingBuffer<T>::size() const
    {
        return m_storage.size();
    }

    template<typename T>
    bool RingBuffer<T>::empty() const
    {
        return m_storage.empty();
    }

    template<typename T>
    bool RingBuffer<T>::full() const
    {
        return m_storage.size() == m_maxSize;
    }

    template<typename T>
    template<typename Pointer>
    void RingBuffer<T>::increment(Pointer& ptr) const
    {
        if (m_lastIdx < 1 || m_storage.empty())
        {
            throw std::logic_error("increment on empty ring-buffer");
        }

        if (ptr == &m_storage[m_lastIdx - 1])
        {
            // end() flag
            ptr = nullptr;
            return;
        }

        const auto* leftBoundPtr = &m_storage.front();
        const auto* rightBoundPtr = &m_storage.back();

        if (m_isRotated && (ptr == rightBoundPtr))
        {
            ptr = const_cast<Pointer>(leftBoundPtr);
        }
        else
        {
            ++ptr;
        }
    }

    template<typename T>
    template<typename Pointer>
    void RingBuffer<T>::decrement(Pointer& ptr) const
    {
        if (m_lastIdx < 1 || m_storage.empty())
        {
            throw std::logic_error("decrement on empty ring-buffer");
        }

        // end() flag check
        if (ptr == nullptr)
        {
            ptr = const_cast<Pointer>(&m_storage[m_lastIdx - 1]);
            return;
        }

        const auto* leftBoundPtr = &m_storage.front();
        const auto* rightBoundPtr = &m_storage.back();

        if (m_isRotated && (ptr == leftBoundPtr))
        {
            ptr = const_cast<Pointer>(rightBoundPtr);
        }
        else
        {
            --ptr;
        }
    }
} // namespace R2C
