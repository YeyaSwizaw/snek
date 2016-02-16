//
// Created by Sam Sleight on 16/02/2016.
//

#ifndef SNEK_SNEK_HPP
#define SNEK_SNEK_HPP

#include <dlfcn.h>

#include <cstdlib>
#include <exception>

namespace snek {

struct LibraryLoadError : public std::exception {
    LibraryLoadError(const char* string) noexcept : message(string) {}

    const char* what() const noexcept {
        return message;
    }

    const char* message;
};

struct SymbolLoadError : public std::exception {
    SymbolLoadError(const char* string) noexcept : message(string) {}

    const char* what() const noexcept {
        return message;
    }

    const char* message;
};

}

#define SNEK_DECLARE_TYPEDEFS(name, ret, ...) \
typedef ret(*name ## _t)(__VA_ARGS__);

#define SNEK_DECLARE_MEMBER(name, ret, ...) \
name ## _t name;

#define SNEK_LOAD_MEMBER(name, ret, ...) \
if(!(this->name = reinterpret_cast<name ## _t>(dlsym(handle, #name)))) { \
    throw snek::SymbolLoadError(dlerror()); \
}

#define SNEK_EXPAND(name) \
struct name { \
    SNEK_FIELDS(SNEK_DECLARE_TYPEDEFS) \
\
    name(const char* library) { \
        handle = dlopen(library, RTLD_LAZY); \
        if(!handle) { \
            throw snek::LibraryLoadError(dlerror()); \
        }\
\
        SNEK_FIELDS(SNEK_LOAD_MEMBER) \
    }; \
\
    ~name() {\
        dlclose(handle); \
    } \
\
    void* handle; \
    SNEK_FIELDS(SNEK_DECLARE_MEMBER) \
};

#endif //SNEK_SNEK_HPP
