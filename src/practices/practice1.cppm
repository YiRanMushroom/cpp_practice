module;
#include <iostream>
export module practice1;

export int practice_fun1() {
    return 2;
}

export template<int num>
void printNum();

template<int num>
void printNum() {
    std::cout << num << std::endl;
}
