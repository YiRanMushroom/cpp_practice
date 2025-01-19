module;
#include <bit>
#include <tuple>
#include <utility>
export module yiran.std_overloads;

// overload for pipe operator, we expect the first argument to be a object
// such that the second argument can be applied to it
// and also the second argument needs to be a rvalue reference

namespace yiran::std_overloads {
    export template<typename T, typename U>
        requires requires(T &&t, U &&u) {
            u(std::forward<T>(t));
        } && (std::is_rvalue_reference_v<U> || std::is_object_v<U>)
    constexpr decltype(auto) operator|(T &&t, U &&u) {
        return u(std::forward<T>(t));
    }

    export constexpr decltype(auto) move() {
        return [](auto &&t) -> decltype(auto) {
            return std::move(t); // NOLINT
        };
    }

    export constexpr decltype(auto) forward() {
        return []<typename Tp>(Tp &&t) -> decltype(auto) {
            return std::forward<Tp>(t);
        };
    }

    export constexpr decltype(auto) deref() {
        return [](auto &&t) -> decltype(auto) {
            return *t;
        };
    }

    export constexpr decltype(auto) ref() {
        return [](auto &&t) -> decltype(auto) {
            return &t;
        };
    }

    export constexpr decltype(auto) get() {
        return [](auto &&t) -> decltype(auto) {
            if constexpr (requires { t.get(); }) {
                return t.get();
            } else {
                return std::get(t);
            }
        };
    }

    export template<auto temp>
    constexpr decltype(auto) get() {
        return [](auto &&t) -> decltype(auto) {
            if constexpr (requires { t.template get<temp>(); }) {
                return t.template get<temp>();
            } else {
                return std::get<temp>(t);
            }
        };
    }

    export constexpr decltype(auto) value_or(auto &&default_value) {
        return [tup = std::forward_as_tuple(std::forward<decltype(default_value)>(default_value))](
            auto &&t) -> decltype(auto) {
            return t.value_or(std::forward<decltype(default_value)>(std::get<0>(tup)));
        };
    }

    export constexpr decltype(auto) begin() {
        return [](auto &&t) -> decltype(auto) {
            return t.begin();
        };
    }

    export constexpr decltype(auto) end() {
        return [](auto &&t) -> decltype(auto) {
            return t.end();
        };
    }

    export template<typename T>
    constexpr decltype(auto) static_cast_to() {
        return [](auto &&t) -> decltype(auto) {
            return static_cast<T>(t);
        };
    }

    export template<typename T>
    constexpr decltype(auto) dynamic_cast_to() {
        return [](auto &&t) -> decltype(auto) {
            return dynamic_cast<T>(t);
        };
    }

    export template<typename T>
    constexpr decltype(auto) const_cast_to() {
        return [](auto &&t) -> decltype(auto) {
            return const_cast<T>(t);
        };
    }

    export template<typename T>
    constexpr decltype(auto) reinterpret_cast_to() {
        return [](auto &&t) -> decltype(auto) {
            return reinterpret_cast<T>(t);
        };
    }

    export template<typename T>
    constexpr decltype(auto) bit_cast_to() {
        return [](auto &&t) -> decltype(auto) {
            return std::bit_cast<T>(t);
        };
    }
}
