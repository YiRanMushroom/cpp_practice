module;
#include <gtest/gtest.h>
#include <iterator>
#include <algorithm>
export module yiran.algorithm;

namespace yiran::algorithm {
    // export template
    export template<typename Iterator, typename Comparator = std::less<void> >
    void quick_sort(const Iterator &begin, const Iterator &end, Comparator &&comp = Comparator{}) {
        std::sort(begin, end, std::forward<Comparator>(comp));
        // NOLINT(misc-no-recursion)
        // if (begin == end || std::next(begin) == end) {
        //     return;
        // }
        //
        // using value_type = typename std::iterator_traits<Iterator>::value_type;
        // const size_t size = std::distance(begin, end);
        //
        // Iterator pivot = begin;
        // std::advance(pivot, size / 2); // may use random later
        //
        // const value_type &pivot_value = *pivot;
        //
        // Iterator left = begin;
        // Iterator right = std::prev(end);
        //
        // while (left <= right) {
        //     while (comp(*left, pivot_value)) {
        //         ++left;
        //     }
        //
        //     while (comp(pivot_value, *right)) {
        //         --right;
        //     }
        //
        //     if (left <= right) {
        //         std::iter_swap(left, right);
        //         ++left;
        //         --right;
        //     }
        // }
        //
        // if (begin < right) {
        //     quick_sort(begin, std::next(right), comp);
        // }
        //
        // if (left < end) {
        //     quick_sort(left, end, comp);
        // }
    }

    export template<typename Container, typename Comparator = std::less<void> >
    void quick_sort(Container &container, Comparator &&comp = Comparator{}) {
        quick_sort(container.begin(), container.end(), std::forward<Comparator>(comp));
    }

    export template<typename Comparator = std::less<void> >
    decltype(auto) quick_sort(Comparator &&comp = Comparator{}) {
        return [tup = std::forward_as_tuple(std::forward<Comparator>(comp))](auto &container) {
            yiran::algorithm::quick_sort(container, std::forward<Comparator>(std::get<0>(tup)));
        };
    }

    export enum class control_flow_hint : uint8_t {
        none = 0,
        request_break
    };

    export template<typename Iterator, typename Predicate>
    void for_each(const Iterator &begin, const Iterator &end, Predicate &&predicate) {
        control_flow_hint hint = control_flow_hint::none;
        for (Iterator it = begin; it != end && hint == control_flow_hint::none; ++it) {
            if constexpr (requires {  predicate(*it, hint);  }) {
                predicate(*it, hint);
            } else {
                predicate(*it);
            }
        }
    }

    export template<typename Container, typename Predicate>
    void for_each(Container &container, Predicate &&predicate) {
        yiran::algorithm::for_each(container.begin(), container.end(), std::forward<Predicate>(predicate));
    }

    export template<typename Predicate>
    decltype(auto) for_each(Predicate &&predicate) {
        return [tup = std::forward_as_tuple(std::forward<Predicate>(predicate))](auto &container) {
            yiran::algorithm::for_each(container, std::forward<Predicate>(std::get<0>(tup)));
        };
    }
}
