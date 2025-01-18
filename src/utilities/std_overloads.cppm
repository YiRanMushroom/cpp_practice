module;
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
    decltype(auto) operator|(T &&t, U &&u) {
        return u(std::forward<T>(t));
    }
}