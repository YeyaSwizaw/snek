#include <iostream>
#include <vector>

#include "snek.hpp"

// Usage Example:
// Define SNEK_FIELDS like follows, where X takes the symbol name, return type, and any parameters
#define SNEK_FIELDS(X) \
    X(test, void) \
    X(add, int, int, int)

// Then call SNEK_EXPAND, providing the struct name
SNEK_EXPAND(thing)
#undef SNEK_FIELDS

int main() {
    try {
        // Load the symbols by calling the constructor, passing the name of the library to load
        auto library = thing("libsnektest.dylib");

        // And huzzah!
        library.test();
        std::cout << library.add(6, 1) << std::endl;

    // Constructor will throw if the library or any of the symbols cannot be loaded
    } catch(std::exception& error) {
        std::cout << error.what();
    }
}

