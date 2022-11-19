#pragma once

// Include the Pebble header
extern "C" {
#include <pebble.h>
}

// Remove the constructor macros that aren't needed in C++
#undef GRect
#undef GSize
#undef GPoint

// Fix for missing snprintf when compiling C++11
extern "C" int snprintf(char *, size_t, const char *, ...);

#include <new>
#include <functional>
#include <memory>
#include <utility>
#include <string>

#define MAIN extern "C" int main(void)
#define VARGS_(_10, _9, _8, _7, _6, _5, _4, _3, _2, _1, N, ...) N 
#define VARGS(...) VARGS_(__VA_ARGS__, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define CONCAT_(a, b) a##b
#define CONCAT(a, b) CONCAT_(a, b)
#define CALLBACK_2(className, methodName) pebble_callback_handler<className, &className::methodName>
#define CALLBACK_1(methodName) CALLBACK_2(std::remove_pointer<decltype(this)>::type, methodName)
#define CALLBACK(...) CONCAT(CALLBACK_, VARGS(__VA_ARGS__))(__VA_ARGS__)
#define HAS_OVERRIDE(baseClassName, methodName) ((void*)(this->*(&baseClassName::methodName)) == (void*)(&baseClassName::methodName)); // TODO remove

#include "pebble_graphics_types.hpp"
#include "pebble_graphics_context.hpp"
#include "pebble_time.hpp"
#include "pebble_layer.hpp"
#include "pebble_clicks.hpp"
#include "pebble_window.hpp"
#include "pebble_text_layer.hpp"
#include "pebble_scroll_layer.hpp"
#include "pebble_event_services.hpp"
#include "pebble_simplemenu_layer.hpp"
#include "pebble_statusbar_layer.hpp"
#include "pebble_menu_layer.hpp"
#include "pebble_actionbar_layer.hpp"
