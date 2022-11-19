#pragma once

#include "pebble.hpp"

namespace Pebble {

class ScrollLayer : public Layer
{  
public:
    ScrollLayer(GRect frame) : Layer((scroll_layer_ = scroll_layer_create(frame), scroll_layer_get_layer(scroll_layer_))) { }
    
    ScrollLayer() : ScrollLayer(GRect(0, 0, 0, 0)) {}
     
    //! Destroys the underlying TextLayer
    virtual ~ScrollLayer() {
        scroll_layer_destroy(scroll_layer_);
    }
    
    //! Instances of this class can be automatically converted to Pebble TextLayers
    inline operator ::ScrollLayer*() { return scroll_layer_; }

    inline void AddChild(Layer& child) { scroll_layer_add_child(scroll_layer_, child); }
    
    inline GPoint GetContentOffset() const { return scroll_layer_get_content_offset(scroll_layer_); }
    
    inline GSize GetContentSize() const { return scroll_layer_get_content_size(scroll_layer_); }
    
    inline bool GetShadowHidden() const { return scroll_layer_get_shadow_hidden(scroll_layer_); }
    
    inline void SetContentOffset(GPoint offset, bool animated) { scroll_layer_set_content_offset(scroll_layer_, offset, animated); }
    inline void SetContentOffset(int offset, bool animated) { SetContentOffset(GPoint(0, offset), animated); }
    
    inline void SetContentSize(GSize size) { scroll_layer_set_content_size(scroll_layer_, size); }
    inline void SetContentSize(int height, int width) { SetContentSize(GSize(height, width)); }
    
    inline void SetFrame(GRect frame) { scroll_layer_set_frame(scroll_layer_, frame); };
    inline void SetFrame(int16_t x, int16_t y, int16_t w, int16_t h) { SetFrame(GRect(x, y, w, h)); }
    inline void SetFrame(GPoint origin,  GSize size) { SetFrame(GRect(origin, size)); }

    inline void SetShadowHidden(bool hidden) { scroll_layer_set_shadow_hidden(scroll_layer_, hidden); }
    
    inline void SetClickConfigOntoWindow(Window& owner_window) { scroll_layer_set_click_config_onto_window(scroll_layer_, owner_window); }

    inline void SetCallbacks(Window& owner_window, ScrollLayerCallbacks callbacks)
    {
        scroll_layer_set_context(scroll_layer_, &owner_window);
        scroll_layer_set_callbacks(scroll_layer_, callbacks);
    }

    inline void SetCallbacks(Window& owner_window, ::ClickConfigProvider click_config_provider, ::ScrollLayerCallback content_offset_changed_handler)
    {
        SetCallbacks(owner_window, {
            .click_config_provider = click_config_provider,
            .content_offset_changed_handler = content_offset_changed_handler
        });
    }
    
    inline void SetCallbacks(Window& owner_window, ::ScrollLayerCallback content_offset_changed_handler)
    {
        SetCallbacks(owner_window, nullptr, content_offset_changed_handler);
    }
    
    inline void SetCallbacks(Window& owner_window, ::ClickConfigProvider click_config_provider)
    {
        SetCallbacks(owner_window, click_config_provider, nullptr);
    }        

protected:
    // The layer pointer
    ::ScrollLayer* scroll_layer_;
};


// Scroll layer callback handler
template<class T, void (T::*Method)(void)>
static void pebble_callback_handler(::ScrollLayer* scroll_layer, void* context)
{
    (((T*)context)->*Method)();
} 

}