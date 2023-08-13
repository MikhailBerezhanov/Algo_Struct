#include <functional>

namespace AlgoStruct
{
    //    i ->           tmp
    // 3  2  1  5  4     (2)
    // <- j
    //
    //       i
    // [2 3] 1  5  4     (1)
    //       j
    //
    //          i
    // [1 2 3]  5  4     (5)
    //          j
    //
    //             i
    // [1 2 3 5]   4     (4)  
    //             j
    //
    // 1  2  3  4  5

    template<class Container, class Comparator = std::greater<typename Container::value_type>>
    void InsertSort(Container& container, Comparator comparator = {})
    {
        if (container.size() < 2)
        {
            return;
        }

        for (typename Container::size_type i = 1; i < container.size(); ++i)
        {
            const auto tmp = container[i];;
            auto j = i;

            while (j > 0 && !comparator(tmp, container[j - 1]))
            {
                container[j] = container[j - 1];
                --j;
            }

            container[j] = tmp;
        }
    }
} // namespace AlgoStruct

