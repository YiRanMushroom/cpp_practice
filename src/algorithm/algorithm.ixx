module;
#include <gtest/gtest.h>
#include <iterator>
#include <algorithm>
export module yiran.algorithm;

import yiran.std_overloads;

namespace yiran::algorithm {
    // export template
    export template<typename Iterator, typename Comparator = std::less<void> >
    constexpr void sort(const Iterator &begin, const Iterator &end, Comparator &&comp = Comparator{}) {
        std::sort(begin, end, std::forward<Comparator>(comp));
        /*NOLINT(misc-no-recursion)
        if (begin == end || std::next(begin) == end) {
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
        }*/
    }

    export template<typename Container, typename Comparator = std::less<void> >
    constexpr void sort(Container &container, Comparator &&comp = Comparator{}) {
        sort(container.begin(), container.end(), std::forward<Comparator>(comp));
    }

    export template<typename Comparator = std::less<void> >
    constexpr decltype(auto) sort(Comparator &&comp = Comparator{}) {
        return [tup = std::forward_as_tuple(std::forward<Comparator>(comp))](auto &container) {
            yiran::algorithm::sort(container, std::forward<Comparator>(std::get<0>(tup)));
        };
    }

    // sorted
    export template<typename Comparator = std::less<void> >
    constexpr decltype(auto) sorted(Comparator &&comp = Comparator{}) {
        return [tup = std::forward_as_tuple(std::forward<Comparator>(comp))](auto &&container) { // NOLINT
            auto result = std::forward<decltype(container)>(container);
            yiran::algorithm::sort(result, std::forward<Comparator>(std::get<0>(tup)));
            return result;
        };
    }

    export enum class control_flow_hint : uint8_t {
        none = 0,
        request_break
    };

    export template<typename Iterator, typename Predicate>
    constexpr void for_each(const Iterator &begin, const Iterator &end, Predicate &&predicate) {
        control_flow_hint hint = control_flow_hint::none;
        for (Iterator it = begin; it != end && hint == control_flow_hint::none; ++it) {
            if constexpr (requires { predicate(*it, hint); }) {
                predicate(*it, hint);
            } else {
                predicate(*it);
            }
        }
    }

    export template<typename Container, typename Predicate>
    constexpr void for_each(Container &container, Predicate &&predicate) {
        yiran::algorithm::for_each(container.begin(), container.end(), std::forward<Predicate>(predicate));
    }

    export template<typename Predicate>
    constexpr decltype(auto) for_each(Predicate &&predicate) {
        return [tup = std::forward_as_tuple(std::forward<Predicate>(predicate))](auto &container) {
            yiran::algorithm::for_each(container, std::forward<Predicate>(std::get<0>(tup)));
        };
    }

    // filter
    template<typename Iterator, typename Predicate>
    constexpr Iterator filter(const Iterator &begin, const Iterator &end, Predicate &&predicate) {
        auto writeIt = begin;
        for (Iterator it = begin; it != end; ++it) {
            if (predicate(*it)) {
                *writeIt = std::move(*it);
                ++writeIt;
            }
        }
        return writeIt;
    }

    template<typename Container, typename Predicate>
    constexpr void filter(Container &container, Predicate &&predicate) {
        auto writeIt = yiran::algorithm::filter(container.begin(), container.end(), std::forward<Predicate>(predicate));
        container.erase(writeIt, container.end());
    }

    export template<typename Predicate>
    constexpr decltype(auto) filter(Predicate &&predicate) {
        return [tup = std::forward_as_tuple(std::forward<Predicate>(predicate))](auto &container) {
            yiran::algorithm::filter(container, std::forward<Predicate>(std::get<0>(tup)));
        };
    }

    // filtered
    export template<typename Predicate>
    constexpr decltype(auto) filtered(Predicate &&predicate) {
        return [tup = std::forward_as_tuple(std::forward<Predicate>(predicate))](auto &&container) {
            auto result = std::forward<decltype(container)>(container);
            yiran::algorithm::filter(result, std::forward<Predicate>(std::get<0>(tup)));
            return result;
        };
    }

    // map
    export template<typename Iterator, typename Mapper>
    constexpr void map(const Iterator &begin, const Iterator &end, Mapper &&mapper) {
        for (auto it = begin; it != end; ++it) {
            *it = mapper(std::move(*it));
        }
    }

    export template<typename Container, typename Mapper>
    constexpr void map(Container &container, Mapper &&mapper) {
        yiran::algorithm::map(container.begin(), container.end(), std::forward<Mapper>(mapper));
    }

    export template<typename Mapper>
    constexpr decltype(auto) map(Mapper &&mapper) {
        return [tup = std::forward_as_tuple(std::forward<Mapper>(mapper))](auto &container) {
            yiran::algorithm::map(container, std::forward<Mapper>(std::get<0>(tup)));
        };
    }

    // mapped
    export template<typename ResultType = void, typename Mapper>
    constexpr decltype(auto) mapped(Mapper &&mapper) {
        return [tup = std::forward_as_tuple(std::forward<Mapper>(mapper))](auto &&container) { // NOLINT
            auto result = [] {
                if constexpr (std::is_same_v<ResultType, void>) {
                    return decltype(container){};
                } else {
                    return ResultType{};
                }
            }();
            if constexpr (requires { result.reserve(std::size(container)); }) {
                result.reserve(std::size(container));
            }

            auto &&mapper = std::get<0>(tup);

            for (auto &&e: container) {
                if constexpr (requires { result.emplace_back(mapper(std::forward<decltype(e)>(e))); }) {
                    result.emplace_back(mapper(std::forward<decltype(e)>(e)));
                } else if constexpr (requires { result.insert(mapper(std::forward<decltype(e)>(e))); }) {
                    result.insert(mapper(std::forward<decltype(e)>(e)));
                } else if constexpr (requires { result.push_back(mapper(std::forward<decltype(e)>(e))); }) {
                    result.push_back(mapper(std::forward<decltype(e)>(e)));
                } else if constexpr (requires { result.push(mapper(std::forward<decltype(e)>(e))); }) {
                    result.push(mapper(std::forward<decltype(e)>(e)));
                } else if constexpr (requires { result.emplace(mapper(std::forward<decltype(e)>(e))); }) {
                    result.emplace(mapper(std::forward<decltype(e)>(e)));
                } else if constexpr (requires { result += mapper(std::forward<decltype(e)>(e)); }) {
                    result += mapper(std::forward<decltype(e)>(e));
                } else {
                    static_assert(std::is_same_v<ResultType, void>,
                                  "mapped: no suitable method found, you can define operator+= for your type");
                }
            }

            return result;
        };
    }

    // accumulate
    export template<typename Iterator, typename Reducer, typename InitialValue>
    constexpr decltype(auto) accumulate(const Iterator &begin, const Iterator &end, Reducer &&reducer,
                                        InitialValue &&initialValue) {
        decltype(auto) result = std::forward<InitialValue>(initialValue);
        for (auto it = begin; it != end; ++it) {
            result = reducer(std::move(result), *it);
        }
        return result;
    }

    export template<typename Container, typename Reducer, typename InitialValue>
    constexpr decltype(auto) accumulate(Container &&container, Reducer &&reducer, InitialValue &&initialValue) {
        return yiran::algorithm::accumulate(container.begin(), container.end(), std::forward<Reducer>(reducer),
                                            std::forward<InitialValue>(initialValue));
    }

    export template<typename Reducer, typename InitialValue>
    constexpr decltype(auto) accumulate(Reducer &&reducer, InitialValue &&initialValue) {
        return [tup = std::forward_as_tuple(std::forward<Reducer>(reducer), std::forward<InitialValue>(initialValue))
                ](auto &&container) {
            return yiran::algorithm::accumulate(container, std::forward<Reducer>(std::get<0>(tup)),
                                                std::forward<InitialValue>(std::get<1>(tup)));
        };
    }

    // no initial value
    export template<typename Iterator, typename Reducer>
    constexpr decltype(auto) reduce(const Iterator &begin, const Iterator &end, Reducer &&reducer) {
        if (begin == end) {
            return typename std::iterator_traits<Iterator>::value_type{};
        }

        auto result = std::move(*begin);

        for (auto it = std::next(begin); it != end; ++it) {
            result = reducer(std::move(result), *it);
        }

        return result;
    }

    export template<typename Container, typename Reducer>
    constexpr decltype(auto) reduce(Container &&container, Reducer &&reducer) {
        return yiran::algorithm::reduce(container.begin(), container.end(), std::forward<Reducer>(reducer));
    }

    export template<typename Reducer>
    constexpr decltype(auto) reduce(Reducer &&reducer) {
        return [tup = std::forward_as_tuple(std::forward<Reducer>(reducer))](auto &&container) {
            return yiran::algorithm::reduce(container, std::forward<Reducer>(std::get<0>(tup)));
        };
    }
}
