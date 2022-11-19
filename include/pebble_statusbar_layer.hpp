#pragma once

#include "pebble.hpp"

namespace Pebble {

class StatusBarLayer : public Layer
{
public:
    StatusBarLayer()
        : Layer((statusbar_layer_ = status_bar_layer_create(), status_bar_layer_get_layer(statusbar_layer_))) { }

    //! Destroys the underlying SimpleMenuLayer
    virtual ~StatusBarLayer() 
    {
         status_bar_layer_destroy(statusbar_layer_);    
    }
    
    //! Instances of this class can be automatically converted to Pebble StatusBarLayer
    inline operator ::StatusBarLayer*() { return statusbar_layer_; } 

    inline GColor GetBackgroundColor() const { return status_bar_layer_get_background_color(statusbar_layer_); }

    inline GColor GetForegroundColor() const { return status_bar_layer_get_foreground_color(statusbar_layer_); }

    inline void SetColors(GColor background, GColor foreground) { status_bar_layer_set_colors(statusbar_layer_, background, foreground); }

    inline void SetSeparatorMode(StatusBarLayerSeparatorMode mode) { status_bar_layer_set_separator_mode(statusbar_layer_, mode); }

protected:
    // The layer pointer
    ::StatusBarLayer* statusbar_layer_;
};

}