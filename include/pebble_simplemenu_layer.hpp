#pragma once

#include "pebble.hpp"

namespace Pebble {

class SimpleMenuLayer : public Layer
{
protected:
    // The layer pointer
    ::SimpleMenuLayer* menu_layer_;
    
public:
    SimpleMenuLayer(Window& window, GRect frame, SimpleMenuSection* sections, int num_sections)
        : Layer((menu_layer_ = simple_menu_layer_create(frame, window, sections, num_sections, &window), simple_menu_layer_get_layer(menu_layer_))) { }

    SimpleMenuLayer(Window& window, SimpleMenuSection* section, int num_sections)
        : SimpleMenuLayer(window, window.GetFrame(), section, num_sections) { }

    template<std::size_t N>
    SimpleMenuLayer(Window& window, GRect frame, SimpleMenuSection (&section)[N])
        : SimpleMenuLayer(window, frame, section, N) { }

    template<std::size_t N>
    SimpleMenuLayer(Window& window, SimpleMenuSection (&section)[N])
        : SimpleMenuLayer(window, section, N) { }

    //! Destroys the underlying SimpleMenuLayer
    virtual ~SimpleMenuLayer() 
    {
        simple_menu_layer_destroy(menu_layer_);    
    }
    
    //! Instances of this class can be automatically converted to Pebble SimpleMenuLayers
    inline operator ::SimpleMenuLayer*() { return menu_layer_; } 

    inline int GetSelectedIndex() const { return simple_menu_layer_get_selected_index(menu_layer_); }

    inline void SetSelectedIndex(int32_t index, bool animated) { simple_menu_layer_set_selected_index(menu_layer_, index, animated); }
};

// Click handler implementation
template<class T, void (T::*Method)(int index)>
static void pebble_callback_handler(int index, void* context)
{
    (((T*)context)->*Method)(index);
}   

}