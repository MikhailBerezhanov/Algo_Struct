#include <utility>
#include <functional>

namespace AlgoStruct
{
    // j ->     <- i
    // 2  4  1  5  3
    //   
    // j        i
    // 2  4  1  3 [5]
    // 
    // j     i
    // 2  3  1 [4] 5
    //
    // j  i
    // 2  1 [3] 4  5
    //
    // 1 [2] 3  4  5

    template<class Container, class Comparator = std::greater<typename Container::value_type>>
    void BubbleSort(Container& container, Comparator comparator = {})
    {
        if (container.size() < 2)
        {
            return;
        }

        bool isSorted = true;

        for (typename Container::size_type i = container.size() - 1; i > 0; --i)
        {
            for (typename Container::size_type j = 0; j < i; ++j)
            {
                if (comparator(container[j], container[i]))
                {
                    using namespace std;
                    swap(container[j], container[i]);
                    isSorted = false;
                }
            }

            if (isSorted)
            {
                return;
            }
        }
    }
} // namespace AlgoStruct
