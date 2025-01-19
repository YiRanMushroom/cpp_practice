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

#ifdef DEFINE_ENTRANCE
int main(int, char **) {
    // fetch_content("www.google.com", "/");

    using namespace yiran::std_overloads;

    constexpr auto db = [] {
        return std::vector{1, 2, 3, 4, 5, 6}
               | yiran::algorithm::filtered([](int a) { return a % 2 == 0; })
               | yiran::algorithm::mapped<std::vector<double> >([](int a)-> double { return a * 2.5; })
               | yiran::algorithm::reduce([](auto &&first, auto &&second) { return first + second; });
    }();

    std::cout << db << std::endl;
}
#endif
