#pragma once

namespace Pebble {

struct GSize : public ::GSize
{
    GSize() {}
    
    GSize(const ::GSize &size)
    {
        w = size.w;
        h = size.h;
    }
    
    GSize(int16_t w, int16_t h)
    {
        this->w = w;
        this->h = h;
    }

    inline operator const ::GSize*() const { return this; }  
    inline operator ::GSize*()  { return this; }  

    inline bool operator== (const ::GSize& size) const { return gsize_equal(this, &size); }
    
    inline bool operator!= (const ::GSize& size) const { return !(*this == size); }
};

//! Represents a point in a 2-dimensional coordinate system.
//! @note Conventionally, the origin of Pebble's 2D coordinate system is in the upper,
//! lefthand corner
//! its x-axis extends to the right and its y-axis extends to the bottom of the screen.
struct GPoint : public ::GPoint
{
    //! Constructs a new GPoint
    GPoint() {}
    
    //! Converts from a Pebble GPoint
    GPoint(const ::GPoint &point)
    {
        x = point.x;
        y = point.y;
    }
    
    //! Constructs a GPoint from X and Y coordinates
    //! @param x The X coordinate 
    //! @param y The Y coordinate
    GPoint(int16_t x, int16_t y)
    {
        this->x = x;
        this->y = y;
    }    

    //! Implicitly converts this instance to Pebble GPoint*
    inline operator const ::GPoint*() const { return this; }  
    inline operator ::GPoint*()  { return this; }  

    //! Tests whether 2 points are equal.
    //! @param point The second point
    //! @return `true` if both points are equal, `false` if not.
    inline bool operator== (const ::GPoint& point) const { return gpoint_equal(this, &point); }
    
    //! Tests whether 2 points are not equal.
    //! @param point The second point
    //! @return `true` if both points are not equal, `false` if not.
    inline bool operator!= (const ::GPoint& point) const { return !(*this == point); }
};

//! Represents a rectangle and defining it using the origin of
//! the upper-lefthand corner and its size.
struct GRect 
{
    //! The coordinate of the upper-lefthand corner point of the rectangle.
    GPoint origin;
    //! The size of the rectangle.
    GSize size;

    //! Constructs an empty GRect
    GRect() {}
    
    //! Converts from Pebble GRect
    GRect(const ::GRect& rect)
    {
        origin.x = rect.origin.x;
        origin.y = rect.origin.y;
        size.w = rect.size.w;
        size.h = rect.size.h;
    }

    //! Constructs from a position and size
    //! @param x The X coordinate
    //! @param y The Y coordinate
    //! @param w The width
    //! @param h The height
    GRect(int16_t x, int16_t y, int16_t w, int16_t h) {
        origin.x = x;
        origin.y = y;
        size.w = w;
        size.h = h;
    }
    
    //! Construct a GRect from a Pebble point and size
    //! @param origin Origin
    //! @param size Size
    GRect(::GPoint origin, ::GSize size) 
    {
        this->origin = origin;
        this->size = size;
    }
    
    //! Construct a GRect from a point and size
    //! @param origin Origin
    //! @param size Size
    GRect(GPoint origin, GSize size)
    {
        this->origin = origin;
        this->size = size;    
    }
    
    //! Convert to a Pebble GRect
    inline operator ::GRect&() { return *(::GRect*)this; }    
    //! Convert to a Pebble GRect
    inline operator const ::GRect&() const { return *(::GRect*)this; }    
    //! Convert to a Pebble GRect pointer
    inline operator ::GRect*() { return (::GRect*)this; }
    //! Convert to a Pebble GRect pointer
    inline operator const ::GRect*() const { return (::GRect*)this; }

    //! Tests whether 2 rectangles are equal.
    //! @param rect The second rectangle
    //! @return `true` if both rectangles are equal, `false` if not.
    inline bool operator== (const ::GRect& rect) const { return grect_equal(*this, &rect); }
    
    //! Tests whether 2 rectangles are not equal.
    //! @param rect The second rectangle
    //! @return `true` if both rectangles are not equal, `false` if equal.
    inline bool operator!= (const ::GRect& rect) const { return !(*this == rect); }   
    
    //! Aligns one rectangle within another rectangle, using an alignment parameter.
    //! The relative coordinate systems of both rectangles are assumed to be the same.
    //! When clip is true, `rect` is also clipped by the constraint.
    //! @param inside_rect The rectangle in which to align `rect`
    //! @param alignment Determines the alignment of `rect` within `inside_rect` by
    //! specifying what edges of should overlap.
    //! @param clip Determines whether `rect` should be trimmed using the edges of `inside_rect`
    //! in case `rect` extends outside of the area that `inside_rect` covers after the alignment.
    inline void Align(const ::GRect& inside_rect, const GAlign alignment, const bool clip) { grect_align(*this, &inside_rect, alignment, clip); }
    
    //! Convenience method to compute the center-point of a given rectangle.
    //! This is equal to `(rect->x + rect->width / 2, rect->y + rect->height / 2)`.
    //! @return The point at the center of `rect`
    inline GPoint CenterPoint() const { return grect_center_point(*this); }    

    //! Returns a rectangle that is shrinked or expanded by the given edge insets.
    //! @note The rectangle is standardized and then the inset parameters are applied.
    //! If the resulting rectangle would have a negative height or width, a GRectZero is returned.
    //! @param insets The insets that will be applied
    //! @return The resulting rectangle
    //! @note Use this function in together with the \ref GEdgeInsets macro
    inline GRect Inset(GEdgeInsets insets) const { return grect_inset(*this, insets); }

    //! Tests whether the size of the rectangle is (0, 0).
    //! @return `true` if the rectangle its size is (0, 0), or `false` if not.
    //! @note If the width and/or height of a rectangle is negative, this
    //! function will return `true`!
    inline bool IsEmpty() { return grect_is_empty(*this); }

    //! Converts a rectangle's values so that the components of its size
    //! (width and/or height) are both positive. In the width and/or height are negative,
    //! the origin will offset, so that the final rectangle overlaps with the original.
    //! For example, a GRect with size (-10, -5) and origin (20, 20), will be standardized
    //! to size (10, 5) and origin (10, 15).
    inline void Standardize() { grect_standardize(*this); }

    //! Trim one rectangle using the edges of a second rectangle.
    //! @param rect_clipper The rectangle of which the edges will serve as "scissors"
    //! in order to trim `rect_to_clip`.
    inline void Clip(const GRect& rect_clipper) { grect_clip(*this, rect_clipper); }

    //! Tests whether a rectangle contains a point.
    //! @param point The point
    //! @return `true` if the rectangle contains the point, or `false` if it does not.
    inline bool ContainsPoint(const GPoint& point) { return grect_contains_point(*this, point); }

    //! Reduce the width and height of a rectangle by insetting each of the edges with
    //! a fixed inset. The returned rectangle will be centered relative to the input rectangle.
    //! @note The function will trip an assertion if the crop yields a rectangle with negative width or height.
    //! @param crop_size_px The inset by which each of the rectangle will be inset.
    //! A positive inset value results in a smaller rectangle, while negative inset value results
    //! in a larger rectangle.
    //! @return The cropped rectangle.
    inline GRect Crop(int32_t crop_size_px) { return grect_crop(*this, crop_size_px); }

    //! Calculates a GPoint located at the angle provided on the perimeter of a circle defined by the
    //! provided GRect.
    //! @param scale_mode Determines how rect will be used to derive the center point and radius.
    //! @param angle The angle at which the point on the circle's perimeter should be calculated.
    //! Use \ref DEG_TO_TRIGANGLE to easily convert degrees to the appropriate value.
    //! @return The point on the circle's perimeter.
    inline GPoint FromPolar(GOvalScaleMode scale_mode, int32_t angle) 
    {
        return gpoint_from_polar(*this, scale_mode, angle);    
    }

    //! Calculates a rectangle centered on the perimeter of a circle at a given angle.
    //! Use this to construct rectangles that follow the perimeter of a circle as an input for
    //! \ref graphics_fill_radial_internal or \ref graphics_draw_arc_internal,
    //! e.g. to draw circles every 30 degrees on a watchface.
    //! @param scale_mode Determines how rect will be used to derive the circle's center point and
    //!        radius.
    //! @param angle The angle at which the point on the circle's perimeter should be calculated.
    //! Use \ref DEG_TO_TRIGANGLE to easily convert degrees to the appropriate value.
    //! @param size Width and height of the desired rectangle.
    //! @return The rectangle centered on the circle's perimeter.
    inline GRect CenterFromPolar(GOvalScaleMode scale_mode, int32_t angle, GSize size) 
    {
        return grect_centered_from_polar(*this, scale_mode, angle, size);
    }     
};


class GBitmapBase 
{
public:
    //! Instances of this class can be automatically converted to Pebble gbitmap pointer
    operator ::GBitmap*() const { return gbitmap_; }

    //! Destructor
    virtual ~GBitmapBase() 
    {
    }

    //! Get the number of bytes per row in the bitmap data for the given \ref GBitmap.
    //! On rectangular displays, this can be used as a safe way of iterating over the rows in the
    //! bitmap, since bytes per row should be set according to format. On circular displays with pixel
    //! format of \ref GBitmapFormat8BitCircular this will return 0, and should not be used for
    //! iteration over frame buffer pixels. Instead, use \ref GBitmapDataRowInfo, which provides safe
    //! minimum and maximum x values for a given row's y value.
    //! @return The number of bytes per row of the GBitmap
    //! @see \ref GetData
    inline uint16_t GetBytesPerRow() const { return gbitmap_get_bytes_per_row(gbitmap_); }

    //! Get the \ref GBitmapFormat for the \ref GBitmap.
    //! @return The format of the given \ref GBitmap.
    inline GBitmapFormat GetFormat() const { return gbitmap_get_format(gbitmap_); }

    //! Get a pointer to the raw image data section of the given \ref GBitmap as specified by the format
    //! of the bitmap.
    //! @return pointer to the raw image data for the GBitmap
    //! @see \ref GetBytesPerRow
    //! @see \ref GBitmap
    inline uint8_t* GetData() const { return gbitmap_get_data(gbitmap_); }

    //! Gets the bounds of the content for the \ref GBitmap. This is set when loading the image or
    //! if changed by \ref gbitmap_set_bounds.
    //! @return The bounding box for the GBitmap.
    //! @see \ref SetBounds
    inline GRect GetBounds() const { return gbitmap_get_bounds(gbitmap_); }

    //! Get the palette for the given \ref GBitmap.
    //! @return Pointer to a \ref GColor array containing the palette colors.
    //! @see \ref gbitmap_set_palette
    inline GColor* GetPalette() const { return gbitmap_get_palette(gbitmap_); }

protected:
    //! Cannot construct 
    GBitmapBase() : gbitmap_(nullptr)
    {
    }

    //! Wrap a Pebble bitmap
    GBitmapBase(::GBitmap* gbitmap) : gbitmap_(gbitmap)
    {
    }

    // Private instance of window pebble Window pointer
    ::GBitmap *gbitmap_; 

    // Allow GBitmap to access the protected members of this class to provide
    // the reference features.
    friend class GBitmap;
}; // End Class


//! An unowned bitmap 
class GBitmap : public GBitmapBase
{
public:
    //! Wrap a Pebble bitmap
    GBitmap(::GBitmap* gbitmap) : GBitmapBase(gbitmap)
    {
    }

    //! Copy bitmap reference
    GBitmap(const GBitmapBase& that) : GBitmapBase(that.gbitmap_)
    {
    }

    //! Assign the bitmap
    GBitmap& operator=(const GBitmapBase& that) 
    {
        gbitmap_ = that.gbitmap_;
        return *this;
    }

    //! Assign the bitmap
    GBitmap& operator=(::GBitmap* gbitmap)
    {
        gbitmap_ = gbitmap;
        return *this;
    }

    //! Returns true if the bitmap is valid and can be used.
    inline bool IsValid() { return gbitmap_ != nullptr; }
}; // End Class


//! A bitmap that destroys the underlying reference when destructed
class GOwnedBitmap : public GBitmapBase
{
public:
    GOwnedBitmap() : GBitmapBase() {}

    //! No copy constructor
    GOwnedBitmap(const GBitmap&) = delete;
    
    //! Move constructor
    GOwnedBitmap(GOwnedBitmap&& that) : GBitmapBase(that.gbitmap_) 
    {
        that.gbitmap_ = nullptr;
    }

    //! Swap assignment operator
    GOwnedBitmap& operator=(GOwnedBitmap that) 
    {
        std::swap(gbitmap_, that.gbitmap_);
        return *this;
    }

    //! Swap assignment operator
    GOwnedBitmap& operator=(GOwnedBitmap& that) 
    {
        std::swap(gbitmap_, that.gbitmap_);
        return *this;
    }

    //! Destroy the bitmap if pointer provided
    virtual ~GOwnedBitmap() 
    {
        if (gbitmap_) gbitmap_destroy(gbitmap_);
    }

    //! Set the bitmap data for the given \ref GBitmap.
    //! @param data A pointer to the bitmap data
    //! @param format the format of the bitmap data. If this is a palettized format, make sure that
    //! there is an accompanying call to \ref gbitmap_set_palette.
    //! @param row_size_bytes How many bytes a single row takes. For example, bitmap data of format
    //! \ref GBitmapFormat1Bit must have a row size as a multiple of 4 bytes.
    //! @param free_on_destroy Set whether the data should be freed when the GBitmap is destroyed.
    inline void SetData(uint8_t * data, GBitmapFormat format, uint16_t row_size_bytes, bool free_on_destroy) 
    {
        gbitmap_set_data(gbitmap_, data, format, row_size_bytes, free_on_destroy);
    }

    //! Set the bounds of the given \ref GBitmap.
    //! @param bounds The bounding box to set.
    //! @see \ref GetBounds
    inline void SetBounds(const GRect& bounds) { gbitmap_set_bounds(gbitmap_, bounds); } 

    //! Set the palette for the given \ref GBitmap.
    //! @param palette The palette to be used. Make sure that the palette is large enough for the
    //! bitmap's format.
    //! @param free_on_destroy Set whether the palette data should be freed when the GBitmap is
    //! destroyed or when another palette is set.
    //! @see \ref GetFormat
    inline void SetPalette(GColor* palette, bool free_on_destroy) { gbitmap_set_palette(gbitmap_, palette, free_on_destroy); }

    //! Create a \ref GBitmap based on raw PNG data.
    //! The resulting \ref GBitmap must be destroyed using \ref gbitmap_destroy().
    //! The developer is responsible for freeing png_data following this call.
    //! @note PNG decoding currently supports 1,2,4 and 8 bit palettized and grayscale images.
    //! @param png_data PNG image data.
    //! @param png_data_size PNG image size in bytes.
    //! @return Bitmap
    static inline GOwnedBitmap CreateFromPngData(const uint8_t * png_data, size_t png_data_size) 
    {
        return gbitmap_create_from_png_data(png_data, png_data_size);
    }

    //! Create a new \ref GBitmap on the heap as a sub-bitmap of a 'base' \ref
    //! GBitmap, using a GRect to indicate what portion of the base to use. The
    //! sub-bitmap will just reference the image data and palette of the base bitmap.
    //! No deep-copying occurs as a result of calling this function, thus the caller
    //! is responsible for making sure the base bitmap and palette will remain available when
    //! using the sub-bitmap. Note that you should not destroy the parent bitmap until
    //! the sub_bitmap has been destroyed.
    //! @param[in] base_bitmap The bitmap that the sub-bitmap of which the image data
    //! will be used by the sub-bitmap
    //! @param sub_rect The rectangle within the image data of the base bitmap. The
    //! bounds of the base bitmap will be used to clip `sub_rect`.
    //! @return A \ref GBitmap. 
    static inline GOwnedBitmap CreateSubBitmap(const ::GBitmap* base_bitmap, ::GRect sub_rect)
    {
         return gbitmap_create_as_sub_bitmap(base_bitmap, sub_rect);
    }
    
    //! Creates a new GBitmap on the heap initialized with the provided Pebble image data.
    //!
    //! When the \ref GBitmap is destroyed but the image data will not be freed automatically. 
    //! The developer is responsible for keeping the image
    //! data in memory as long as the bitmap is used and releasing it after the bitmap is destroyed.
    //! @note One way to generate Pebble image data is to use bitmapgen.py in the Pebble
    //! SDK to generate a .pbi file.
    //! @param data The Pebble image data. Must not be NULL. The function
    //! assumes the data to be correct; there are no sanity checks performed on the
    //! data. The data will not be copied and the pointer must remain valid for the
    //! lifetime of this GBitmap.
    //! @return The \ref GBitmap.
    static inline GOwnedBitmap CreateWithData(const uint8_t* data) 
    {
        return gbitmap_create_with_data(data);
    }
    
    //! Creates a new \ref GBitmap on the heap using a Pebble image file stored as a resource.
    //! @param resource_id The ID of the bitmap resource to load
    //! @return The \ref GBitmap.
    static inline GOwnedBitmap CreateWithResource(uint32_t resource_id)
    {
        return gbitmap_create_with_resource(resource_id);        
    }

    //! Creates a new blank GBitmap on the heap initialized to zeroes.
    //! In the case that the format indicates a palettized bitmap, a palette of appropriate size will
    //! also be allocated on the heap.
    //! @param size The Pebble image dimensions as a \ref GSize.
    //! @param format The \ref GBitmapFormat the created image should be in.
    //! @return The \ref GBitmap. 
    static inline GOwnedBitmap CreateBlank(GSize size, GBitmapFormat format)
    {
        return gbitmap_create_blank(size, format);
    }

    //! Creates a new blank GBitmap on the heap, initialized to zeroes, and assigns it the given
    //! palette.
    //! No deep-copying of the palette occurs, so the caller is responsible for making sure the palette
    //! remains available when using the resulting bitmap. Management of that memory can be handed off
    //! to the system with the free_on_destroy argument.
    //! @param size The Pebble image dimensions as a \ref GSize.
    //! @param format the \ref GBitmapFormat the created image and palette should be in.
    //! @param palette a pointer to a palette that is to be used for this GBitmap. The palette should
    //! be large enough to hold enough colors for the specified format. For example,
    //! \ref GBitmapFormat2BitPalette should have 4 colors, since 2^2 = 4.
    //! @param free_on_destroy Set whether the palette data should be freed along with the bitmap data
    //! when the GBitmap is destroyed.
    //! @return The \ref GBitmap. 
    static inline GOwnedBitmap CreateBlank(GSize size, GBitmapFormat format, GColor* palette, bool free_on_destroy)
    {
        return gbitmap_create_blank_with_palette(size, format, palette, free_on_destroy);
    }

    //! Given a 1-bit GBitmap, create a new bitmap of format GBitmapFormat1BitPalette.
    //! The new data buffer is allocated on the heap, and a 2-color palette is allocated as well.
    //! @param src_bitmap A GBitmap of format GBitmapFormat1Bit which is to be copied into a newly
    //! created GBitmap of format GBitmapFormat1BitPalettized.
    //! @returns The newly created 1-bit palettized GBitmap.
    //! @note The new bitmap does not depend on any data from src_bitmap, so src_bitmap can be freed
    //! without worry.
    static inline GOwnedBitmap CreatePalettizedFrom1bit(const ::GBitmap* src_bitmap)
    {
        return gbitmap_create_palettized_from_1bit(src_bitmap);
    }

protected:
    //! Create bitmap as sub bitmap
    GOwnedBitmap(::GBitmap* bitmap) : GBitmapBase(bitmap)
    {
    }

}; // End Class


}