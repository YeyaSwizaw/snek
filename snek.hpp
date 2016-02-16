//
// Created by Sam Sleight on 16/02/2016.
//

#ifndef SNEK_SNEK_HPP
#define SNEK_SNEK_HPP

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

void* load_library(const char* library) throw(LibraryLoadError);

void unload_library(void* handle) noexcept;

template<typename T>
T load_symbol(void* handle, const char* symbol) throw(SymbolLoadError);

}

#ifdef _WIN32

namespace snek {

void* load_library(const char* library) throw(LibraryLoadError) {
    throw LibraryLoadError("Not yet implemented on Windows");
}

template<typename T>
T load_symbol(void* handle, const char* symbol) throw(SymbolLoadError) {
    throw SymbolLoadError("Not yet implemented on Windows");
}

void unload_library(void* handle) noexcept {

}

}

#else

#include <dlfcn.h>

namespace snek {

void* load_library(const char* library) throw(LibraryLoadError) {
    void* handle = dlopen(library, RTLD_LAZY);
    if (!handle) {
        throw LibraryLoadError(dlerror());
    }

    return handle;
}

template<typename T>
T load_symbol(void* handle, const char* symbol) throw(SymbolLoadError) {
    void* sym = dlsym(handle, symbol);
    if (!sym) {
        throw SymbolLoadError(dlerror());
    }

    return reinterpret_cast<T>(sym);
}

void unload_library(void* handle) noexcept {
    dlclose(handle);
}

}

#endif

#define SNEK_DECLARE_TYPEDEFS(name, ret, ...) \
typedef ret(*name ## _t)(__VA_ARGS__);

#define SNEK_DECLARE_MEMBER(name, ret, ...) \
name ## _t name;

#define SNEK_LOAD_MEMBER(name, ret, ...) \
this->name = snek::load_symbol<name ## _t>(this->handle, #name);

#define SNEK_EXPAND(name) \
struct name { \
    SNEK_FIELDS(SNEK_DECLARE_TYPEDEFS) \
\
    name(const char* library) throw(snek::LibraryLoadError, snek::SymbolLoadError) { \
        this->handle = snek::load_library(library); \
        SNEK_FIELDS(SNEK_LOAD_MEMBER) \
    }; \
\
    ~name() {\
        snek::unload_library(this->handle); \
    } \
\
    void* handle; \
    SNEK_FIELDS(SNEK_DECLARE_MEMBER) \
};

#endif //SNEK_SNEK_HPP
