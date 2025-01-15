module;

#include <iostream>
#include <vector>
#include <boost/asio.hpp>

export module main;
import quick_sort;
import boost_asio;

int main(int, char **) {
    fetch_content("www.google.com", "/");
}
