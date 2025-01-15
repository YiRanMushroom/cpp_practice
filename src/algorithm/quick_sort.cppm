module;

#include <iterator>
#include <utility>
#include <random>

export module quick_sort;

namespace algorithm {
    // export template
    export template<typename Iterator, typename Comparator = std::less<void> >
    void quick_sort(const Iterator &begin, const Iterator &end, Comparator &&comp = Comparator{}) {
        if (begin >= end) {
            return;
        }

        using value_type = typename std::iterator_traits<Iterator>::value_type;
        const size_t size = std::distance(begin, end);

        Iterator pivot = begin;
        std::advance(pivot, size / 2); // may use random later

        const value_type &pivot_value = *pivot;

        Iterator left = begin;
        Iterator right = std::prev(end);

        while (left <= right) {
            while (comp(*left, pivot_value)) {
                ++left;
            }

            while (comp(pivot_value, *right)) {
                --right;
            }

            if (left <= right) {
                std::iter_swap(left, right);
                ++left;
                --right;
            }
        }

        if (begin < right) {
            quick_sort(begin, std::next(right), comp);
        }

        if (left < end) {
            quick_sort(left, end, comp);
        }
    }
}
