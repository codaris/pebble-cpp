#pragma once

#include "pebble.hpp"

namespace Pebble {

//! @addtogroup TextDrawing Drawing Text
//!   \brief Functions to draw text into a graphics context
//!
//! See \ref GraphicsContext for more information about the graphics context.
//!
//! Other drawing functions and related documentation:
//! * \ref Drawing
//! * \ref PathDrawing
//! * \ref GraphicsTypes
//! @{

class GTextAttributes
{
public:
    //! Creates an instance of GTextAttributes for advanced control when rendering text.
    GTextAttributes() : text_attributes_(graphics_text_attributes_create()) {};    

    //! Instances of this class can be automatically converted to Pebble GTextAttributes pointer
    operator ::GTextAttributes*() { return text_attributes_; }    
    operator ::GTextAttributes*() const { return text_attributes_; }    

    //! Destroys the underlying pebble GTextAttributes
    virtual ~GTextAttributes() 
    { 
        graphics_text_attributes_destroy(text_attributes_);
    }

    //! Restores text flow to the rectangular default.
    //! @see EnableScreenTextFlow
    //! @see text_layer_restore_default_text_flow_and_paging
    inline void RestoreDefaultTextFlow() { graphics_text_attributes_restore_default_text_flow(text_attributes_); }

    //! Enables text flow that follows the boundaries of the screen.
    //! @param inset Additional amount of pixels to inset to the inside of the screen for text flow
    //! calculation. Can be zero.
    //! @see graphics_text_attributes_restore_default_text_flow
    //! @see text_layer_enable_screen_text_flow_and_paging
    inline void EnableScreenTextFlow(uint8_t inset) { graphics_text_attributes_enable_screen_text_flow(text_attributes_, inset); } 

    //! Restores paging and locked content origin to the defaults.
    //! @see graphics_text_attributes_enable_paging
    //! @see text_layer_restore_default_text_flow_and_paging
    inline void RestoreDefaultPaging() { graphics_text_attributes_restore_default_paging(text_attributes_); }

    //! Enables paging and locks the text flow calculation to a fixed point on the screen.
    //! @param content_origin_on_screen Absolute coordinate on the screen where the text content
    //!     starts before an animation or scrolling takes place. Usually the frame's origin of a layer
    //!     in screen coordinates.
    //! @param paging_on_screen Rectangle in absolute coordinates on the screen that describes where
    //!     text content pages. Usually the container's absolute frame in screen coordinates.
    //! @see graphics_text_attributes_restore_default_paging
    //! @see graphics_text_attributes_enable_screen_text_flow
    //! @see text_layer_enable_screen_text_flow_and_paging
    //! @see layer_convert_point_to_screen
    inline void EnablePaging(GPoint content_origin_on_screen, GRect paging_on_screen) { graphics_text_attributes_enable_paging(text_attributes_, content_origin_on_screen, paging_on_screen); }

protected:
    //! Underlying GTextAttributes
    ::GTextAttributes* text_attributes_;
};
//! @}


//! @addtogroup GraphicsContext Graphics Context
//! @{

class GContext 
{
public:
    //! Wraps an existing graphics context
    GContext(::GContext *ctx) : gcontext_(ctx) {};    

    virtual ~GContext() { }
    
    //! Instances of this class can be automatically converted to Pebble GContext pointer
    operator ::GContext*() const { return gcontext_; }

    //! Sets the current stroke color of the graphics context.
    //! @param color The new stroke color
    inline void SetStrokeColor(GColor color) const { graphics_context_set_stroke_color(gcontext_, color); }

    //! Sets the current fill color of the graphics context.
    //! @param color The new fill color
    inline void SetFillColor(GColor color) const { graphics_context_set_fill_color(gcontext_, color); }

    //! Sets the current text color of the graphics context.
    //! @param color The new text color
    inline void SetTextColor(GColor color) const { graphics_context_set_text_color(gcontext_, color); }

    //! Sets the current bitmap compositing mode of the graphics context.
    //! @param mode The new compositing mode
    //! @see \ref GCompOp
    //! @see \ref bitmap_layer_set_compositing_mode()
    //! @note At the moment, this only affects the bitmaps drawing operations
    //! -- \ref graphics_draw_bitmap_in_rect(), \ref graphics_draw_rotated_bitmap, and
    //! anything that uses those APIs --, but it currently does not affect the filling or stroking
    //! operations.
    inline void SetCompositingMode(GCompOp mode) const { graphics_context_set_compositing_mode(gcontext_, mode); }

    //! Sets whether antialiasing is applied to stroke drawing
    //! @param enable True = antialiasing enabled, False = antialiasing disabled
    //! @note Default value is true.
    inline void SetAntialiased(bool enable) const { graphics_context_set_antialiased(gcontext_, enable); }

    //! Sets the width of the stroke for drawing routines
    //! @param stroke_width Width in pixels of the stroke.
    //! @note If stroke width of zero is passed, it will be ignored and will not change the value
    //! stored in GContext. Currently, only odd stroke_width values are supported. If an even value
    //! is passed in, the value will be stored as is, but the drawing routines will round down to the
    //! previous integral value when drawing. Default value is 1.
    inline void SetStrokeWidth(uint8_t stroke_width) const { graphics_context_set_stroke_width(gcontext_, stroke_width); }

    //! Draw text into the current graphics context, using the context's current text color.
    //! The text will be drawn inside a box with the specified dimensions and
    //! configuration, with clipping occuring automatically.
    //! @param text The zero terminated UTF-8 string to draw
    //! @param font The font in which the text should be set
    //! @param box The bounding box in which to draw the text. The first line of text will be drawn
    //! against the top of the box.
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits inside
    //! the box.
    //! @param alignment The horizontal alignment of the text
    inline void DrawText(const char* text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode = GTextOverflowModeWordWrap, const GTextAlignment alignment = GTextAlignmentLeft) const
    {
        graphics_draw_text(gcontext_, text, font, box, overflow_mode, alignment, nullptr);
    }

    //! Draw text into the current graphics context, using the context's current text color.
    //! The text will be drawn inside a box with the specified dimensions and
    //! configuration, with clipping occuring automatically.
    //! @param text The zero terminated UTF-8 string to draw
    //! @param font The font in which the text should be set
    //! @param box The bounding box in which to draw the text. The first line of text will be drawn
    //! against the top of the box.
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits inside
    //! the box.
    //! @param alignment The horizontal alignment of the text
    //! @param text_attributes Optional text attributes to describe the characteristics of the text
    inline void DrawText(const char* text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, const GTextAttributes& text_attributes ) const
    {
        graphics_draw_text(gcontext_, text, font, box, overflow_mode, alignment, text_attributes);
    }

    //! Draw text into the current graphics context, using the context's current text color.
    //! The text will be drawn inside a box with the specified dimensions and
    //! configuration, with clipping occuring automatically.
    //! @param text The zero terminated UTF-8 string to draw
    //! @param font The font in which the text should be set
    //! @param box The bounding box in which to draw the text. The first line of text will be drawn
    //! against the top of the box.
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits inside
    //! the box.
    //! @param alignment The horizontal alignment of the text
    inline void DrawText(const std::string& text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode = GTextOverflowModeWordWrap, const GTextAlignment alignment = GTextAlignmentLeft) const
    {
        graphics_draw_text(gcontext_, text.c_str(), font, box, overflow_mode, alignment, nullptr);
    }

    //! Draw text into the current graphics context, using the context's current text color.
    //! The text will be drawn inside a box with the specified dimensions and
    //! configuration, with clipping occuring automatically.
    //! @param text The zero terminated UTF-8 string to draw
    //! @param font The font in which the text should be set
    //! @param box The bounding box in which to draw the text. The first line of text will be drawn
    //! against the top of the box.
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits inside
    //! the box.
    //! @param alignment The horizontal alignment of the text
    //! @param text_attributes Optional text attributes to describe the characteristics of the text
    inline void DrawText(const std::string& text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, const GTextAttributes& text_attributes) const
    {
        graphics_draw_text(gcontext_, text.c_str(), font, box, overflow_mode, alignment, text_attributes);
    }

    //! Obtain the maximum size that a text with given font, overflow mode and alignment occupies
    //! within a given rectangular constraint.
    //! @param text The zero terminated UTF-8 string for which to calculate the size
    //! @param font The font in which the text should be set while calculating the size
    //! @param box The bounding box in which the text should be constrained
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits
    //! inside the box.
    //! @param alignment The horizontal alignment of the text
    //! @return The maximum size occupied by the text
    //! @see app_graphics_text_layout_get_content_size_with_attributes
    static inline GSize GetContentSize(const char* text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode = GTextOverflowModeWordWrap, const GTextAlignment alignment = GTextAlignmentLeft) 
    {
        return graphics_text_layout_get_content_size(text, font, box, overflow_mode, alignment);
    }

    //! Obtain the maximum size that a text with given font, overflow mode and alignment occupies
    //! within a given rectangular constraint.
    //! @param text The zero terminated UTF-8 string for which to calculate the size
    //! @param font The font in which the text should be set while calculating the size
    //! @param box The bounding box in which the text should be constrained
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits
    //! inside the box.
    //! @param alignment The horizontal alignment of the text
    //! @param text_attributes Optional text attributes to describe the characteristics of the text
    //! @return The maximum size occupied by the text
    //! @see app_graphics_text_layout_get_content_size
    static inline GSize GetContentSize(const char* text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, const GTextAttributes& text_attributes)
    {
        return graphics_text_layout_get_content_size_with_attributes(text, font, box, overflow_mode, alignment, text_attributes);
    }

    //! Obtain the maximum size that a text with given font, overflow mode and alignment occupies
    //! within a given rectangular constraint.
    //! @param text The zero terminated UTF-8 string for which to calculate the size
    //! @param font The font in which the text should be set while calculating the size
    //! @param box The bounding box in which the text should be constrained
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits
    //! inside the box.
    //! @param alignment The horizontal alignment of the text
    //! @return The maximum size occupied by the text
    //! @see app_graphics_text_layout_get_content_size_with_attributes
    static inline GSize GetContentSize(const std::string& text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode = GTextOverflowModeWordWrap, const GTextAlignment alignment = GTextAlignmentLeft) 
    {
        return graphics_text_layout_get_content_size(text.c_str(), font, box, overflow_mode, alignment);
    }

    //! Obtain the maximum size that a text with given font, overflow mode and alignment occupies
    //! within a given rectangular constraint.
    //! @param text The zero terminated UTF-8 string for which to calculate the size
    //! @param font The font in which the text should be set while calculating the size
    //! @param box The bounding box in which the text should be constrained
    //! @param overflow_mode The overflow behavior, in case the text is larger than what fits
    //! inside the box.
    //! @param alignment The horizontal alignment of the text
    //! @param text_attributes Optional text attributes to describe the characteristics of the text
    //! @return The maximum size occupied by the text
    //! @see app_graphics_text_layout_get_content_size
    static inline GSize GetContentSize(const std::string& text, GFont const font, const GRect box, const GTextOverflowMode overflow_mode, const GTextAlignment alignment, const GTextAttributes& text_attributes) 
    {
        return graphics_text_layout_get_content_size_with_attributes(text.c_str(), font, box, overflow_mode, alignment, text_attributes);
    }

    //! Draws a pixel at given point in the current stroke color
    //! @param point The point at which to draw the pixel
    inline void DrawPixel(GPoint point) const { graphics_draw_pixel(gcontext_, point); }

    //! Draws line in the current stroke color, current stroke width and AA flag
    //! @param p0 The starting point of the line
    //! @param p1 The ending point of the line
    inline void DrawLine(GPoint p0, GPoint p1) const { graphics_draw_line(gcontext_, p0, p1); }

    //! Draws a 1-pixel wide rectangle outline in the current stroke color
    //! @param rect The rectangle for which to draw the outline
    inline void DrawRect(GRect rect) const { graphics_draw_rect(gcontext_, rect); }

    //! Fills a rectangle with the current fill color, optionally rounding all or a
    //! selection of its corners.
    //! @param ctx The destination graphics context in which to draw
    //! @param rect The rectangle to fill
    //! @param corner_radius The rounding radius of the corners in pixels (maximum is 8 pixels)
    //! @param corner_mask Bitmask of the corners that need to be rounded.
    //! @see \ref GCornerMask
    inline void FillRect(GRect rect, uint16_t corner_radius, GCornerMask corner_mask) const { graphics_fill_rect(gcontext_, rect, corner_radius, corner_mask); }

    //! Draws the outline of a circle in the current stroke color
    //! @param p The center point of the circle
    //! @param radius The radius in pixels
    inline void DrawCircle(GPoint p, uint16_t radius) const { graphics_draw_circle(gcontext_, p, radius); }

    //! Fills a circle in the current fill color
    //! @param p The center point of the circle
    //! @param radius The radius in pixels
    inline void FillCircle(GPoint p, uint16_t radius) const { graphics_fill_circle(gcontext_, p, radius); }

    //! Draws the outline of a rounded rectangle in the current stroke color
    //! @param rect The rectangle defining the dimensions of the rounded rectangle to draw
    //! @param radius The corner radius in pixels
    inline void DrawRoundRect(GRect rect, uint16_t radius) const { graphics_draw_round_rect(gcontext_, rect, radius); }

    //! Draws a bitmap into the graphics context, inside the specified rectangle
    //! @param bitmap The bitmap to draw
    //! @param rect The rectangle in which to draw the bitmap
    //! @note If the size of `rect` is smaller than the size of the bitmap,
    //! the bitmap will be clipped on right and bottom edges.
    //! If the size of `rect` is larger than the size of the bitmap,
    //! the bitmap will be tiled automatically in both horizontal and vertical
    //! directions, effectively drawing a repeating pattern.
    //! @see GBitmap
    //! @see GContext
    inline void DrawBitmapInRect(const GBitmap& bitmap, GRect rect) const { graphics_draw_bitmap_in_rect(gcontext_, bitmap, rect); }

    //! A shortcut to capture the framebuffer in the native format of the watch.
    inline GBitmap CaptureFrameBuffer() { return graphics_capture_frame_buffer(gcontext_); }
    
    //! Captures the frame buffer for direct access, using the given format.
    //! Graphics functions will not affect the frame buffer while it is captured.
    //! The frame buffer is released when {@link ReleaseFrameBuffer} is called.
    //! The frame buffer must be released before the end of a layer's `.update_proc`
    //! for the layer to be drawn properly.
    //!
    //! While the frame buffer is captured calling {@link CaptureFrameBuffer}
    //! will fail and return and invalid GBitmap
    //! @note When writing to the frame buffer, you should respect the visible boundaries of a
    //! window on the screen. Use layer_get_frame(window_get_root_layer(window)).origin to obtain its
    //! position relative to the frame buffer. For example, drawing to (5, 5) in the frame buffer
    //! while the window is transitioning to the left with its origin at (-20, 0) would
    //! effectively draw that point at (25, 5) relative to the window. For this reason you should
    //! consider the window's root layer frame when calculating drawing coordinates.
    //! @see GBitmap
    //! @see GBitmapFormat
    //! @see layer_get_frame
    //! @see window_get_root_layer
    //! @param ctx The graphics context providing the frame buffer
    //! @param format The format in which the framebuffer should be captured. Supported formats
    //! are \ref GBitmapFormat1Bit and \ref GBitmapFormat8Bit.
    //! @return A pointer to the frame buffer. Invalid if failed.
    inline GBitmap CaptureFrameBuffer(GBitmapFormat format) { return graphics_capture_frame_buffer_format(gcontext_, format); }

    //! Releases the frame buffer.
    //! Must be called before the end of a layer's `.update_proc` for the layer to be drawn properly.
    //!
    //! If `buffer` does not point to the address previously returned by
    //! {@link graphics_capture_frame_buffer} the frame buffer will not be released.
    //! @param buffer The frame buffer bitmap
    //! @return True if the frame buffer was released successfully
    inline bool ReleaseFrameBuffer(const GBitmap& buffer) { return graphics_release_frame_buffer(gcontext_, buffer); }

    //! Whether or not the frame buffer has been captured by {@link graphics_capture_frame_buffer}.
    //! Graphics functions will not affect the frame buffer until it has been released by
    //! {@link graphics_release_frame_buffer}.
    //! @return True if the frame buffer has been captured
    inline bool FrameBufferIsCaptured() { return graphics_frame_buffer_is_captured(gcontext_); }

    //! Draws a rotated bitmap with a memory-sensitive 2x anti-aliasing technique
    //! (using ray-finding instead of super-sampling), which is thresholded into a b/w bitmap for 1-bit
    //! and color blended for 8-bit.
    //! @note This API has performance limitations that can degrade user experience. Use sparingly.
    //! @param src The source bitmap to draw
    //! @param src_ic Instance center (single point unaffected by rotation) relative to source bitmap
    //! @param rotation Angle of rotation. Rotation is an integer between 0 (no rotation)
    //! and TRIG_MAX_ANGLE (360 degree rotation). Use \ref DEG_TO_TRIGANGLE to easily convert degrees
    //! to the appropriate value.
    //! @param dest_ic Where to draw the instance center of the rotated bitmap in the context.
    inline void DrawRotatedBitmap(const GBitmap& src, GPoint src_ic, int rotation, GPoint dest_ic) const 
    { 
        graphics_draw_rotated_bitmap(gcontext_, src, src_ic, rotation, dest_ic); 
    }    

    //! Draws a line arc clockwise between `angle_start` and `angle_end`, where 0° is
    //! the top of the circle. If the difference between `angle_start` and `angle_end` is greater
    //! than 360°, a full circle will be drawn.
    //! @param rect The reference rectangle to derive the center point and radius (see scale_mode).
    //! @param scale_mode Determines how rect will be used to derive the center point and radius.
    //! @param angle_start Radial starting angle. Use \ref DEG_TO_TRIGANGLE to easily convert degrees
    //! to the appropriate value.
    //! @param angle_end Radial finishing angle. If smaller than `angle_start`, nothing will be drawn.
    inline void DrawArc(GRect rect, GOvalScaleMode scale_mode, int32_t angle_start, int32_t angle_end) const 
    {
        graphics_draw_arc(gcontext_, rect, scale_mode, angle_start, angle_end);
    }

    //! Fills a circle clockwise between `angle_start` and `angle_end`, where 0° is
    //! the top of the circle. If the difference between `angle_start` and `angle_end` is greater
    //! than 360°, a full circle will be drawn and filled. If `angle_start` is greater than
    //! `angle_end` nothing will be drawn.
    //! @note A simple example is drawing a 'Pacman' shape, with a starting angle of -225°, and
    //! ending angle of 45°. By setting `inset_thickness` to a non-zero value (such as 30) this
    //! example will produce the letter C.
    //! @param rect The reference rectangle to derive the center point and radius (see scale).
    //! @param scale_mode Determines how rect will be used to derive the center point and radius.
    //! @param inset_thickness Describes how thick in pixels the radial will be drawn towards its
    //!        center measured from the outside.
    //! @param angle_start Radial starting angle. Use \ref DEG_TO_TRIGANGLE to easily convert degrees
    //! to the appropriate value.
    //! @param angle_end Radial finishing angle. If smaller than `angle_start`, nothing will be drawn.
    inline void FillRadial(GRect rect, GOvalScaleMode scale_mode, uint16_t inset_thickness, int32_t angle_start, int32_t angle_end) const 
    { 
        graphics_fill_radial(gcontext_, rect, scale_mode, inset_thickness, angle_start, angle_end);    
    }

protected:
    ::GContext* gcontext_;
};

//! @}

}