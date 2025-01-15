module;

#include <iostream>
#include <vector>
#include <boost/asio.hpp>

export module main;
import quick_sort;
import boost_asio;

int main(int, char **) {
    fetch_content("www.google.com", "/");

    std::tuple tp{1, 2, 3, 4, 5};
    auto &&[a,b,c,d,e]{tp};

}
