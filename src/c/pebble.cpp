extern "C" {
#include <pebble.h>
}

#include <memory>
#include <utility>
#include <new>

//! Implementation of new operator
void* operator new(size_t size) {
    return malloc(size);
}

//! Implementation of delete operator
void operator delete(void *ptr) {
    free(ptr);
}

//! Call to pure virtual function (NOP)
extern "C" void __cxa_pure_virtual() {  }

// Included to type-check the pebble files
#include "pebble.hpp"
