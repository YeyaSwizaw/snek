//
// Created by Sam Sleight on 16/02/2016.
//

#include <iostream>

extern "C" void test() {
    std::cout << "Hello World!" << std::endl;
}

extern "C" int add(int x, int y) {
    return x + y;
}
