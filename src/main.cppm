module;

#include <iostream>
#include <vector>
#include <boost/asio.hpp>
#include <format>

export module main;
import yiran.algorithm;
import boost_asio;
import practice1;
import yiran.std_overloads;

constexpr std::string constexpr_to_string(int v) {
    // we need our own implementation of to_string to be constexpr
    // because std::to_string is not constexpr
    std::string result;
    if (v < 0) {
        result.push_back('-');
        v = -v;
    }
    do {
        result.push_back(v % 10 + '0');
        v /= 10;
    } while (v);
    return result;
}

// consteval bool is_const_eval(auto &&... args) {
//     (((void) args), ...);
//     return true;
// }

consteval const char *string_to_const_char(const std::string &str) {
    return str.c_str();
}

#ifdef DEFINE_ENTRANCE
int main(int, char **) {
    using namespace yiran::std_overloads;
    static_assert(![] consteval {
        return std::vector{1, 2, 3, 4, 5, 6}
               | yiran::algorithm::filtered([](int a) { return a % 2 == 0; })
               | yiran::algorithm::mapped([](int a)-> int { return a * 5; })
               | yiran::algorithm::mapped<std::vector<std::string> >([](int a) {
                   return std::string{"value: "} + constexpr_to_string(a) + "\n";
               })
               | yiran::algorithm::reduce([](auto &&first, auto &&second) { return first + second; });
    }().empty());

    operator<<(std::cout, std::vector{1, 2, 3, 4, 5, 6}
                          | yiran::algorithm::filtered([](int a) { return a % 2 == 0; })
                          | yiran::algorithm::mapped([](int a)-> int { return a * 5; })
                          | yiran::algorithm::mapped<std::vector<std::string> >([](int a) {
                              return std::format("value: {}\n", a);
                          })
                          | yiran::algorithm::reduce([](auto &&first, auto &&second) {
                              return first + second;
                          }));
}
#endif
