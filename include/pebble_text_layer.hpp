#pragma once

namespace Pebble {

class TextLayer : public Layer
{
public:
    TextLayer(GRect frame) : Layer((text_layer_ = text_layer_create(frame), text_layer_get_layer(text_layer_))) { }
    
    TextLayer() : TextLayer(GRect(0, 0, 0, 0)) {}
     
    //! Destroys the underlying TextLayer
    virtual ~TextLayer() {
        text_layer_destroy(text_layer_);    
    }
    
    //! Instances of this class can be automatically converted to Pebble TextLayers
    inline operator ::TextLayer*() { return text_layer_; } 

    inline GSize GetContentSize() const { return text_layer_get_content_size(text_layer_); }
    
    inline const char* GetText() const { return text_layer_get_text(text_layer_); }

    inline void SetBackgroundColor(GColor color) { text_layer_set_background_color(text_layer_, color); }
    
    inline void SetTextColor(GColor color) { text_layer_set_text_color(text_layer_, color); }
    
    inline void SetText(const char* text) { text_layer_set_text(text_layer_, text); }
    
    inline void SetFont(GFont font) { text_layer_set_font(text_layer_, font); }

    inline void SetFont(const char* font_key) { SetFont(fonts_get_system_font(font_key)); }
   
    inline void SetOverflowMode(GTextOverflowMode line_mode) { text_layer_set_overflow_mode(text_layer_, line_mode); }

    inline void SetSize(const GSize max_size) { text_layer_set_size(text_layer_, max_size); }
    
    inline void SetTextAlignment(GTextAlignment text_alignment) { text_layer_set_text_alignment(text_layer_, text_alignment); }
    
    inline void SizeToContent() { text_layer_set_size(text_layer_,  text_layer_get_content_size(text_layer_)); }

protected:
    // The layer pointer
    ::TextLayer *text_layer_; 
};

}