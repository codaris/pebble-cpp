#pragma once

#include "pebble.hpp"

namespace Pebble {

// Pebble callback handlers for menu items
// ---------------------------------------
template<class T, uint16_t (T::*Method)(void)>
static uint16_t pebble_callback_handler(struct ::MenuLayer *menu_layer, void *callback_context)
{
    return (((T*)callback_context)->*Method)();
} 

template<class T, uint16_t (T::*Method)(uint16_t)>
static uint16_t pebble_callback_handler(struct ::MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
    uint16_t result = (((T*)callback_context)->*Method)(section_index);
    return result;    
} 

template<class T, int16_t (T::*Method)(const MenuIndex&)>
static int16_t pebble_callback_handler(::MenuLayer *menu_layer, ::MenuIndex *cell_index, void *callback_context)
{
    return (((T*)callback_context)->*Method)(*cell_index);
} 

template<class T, int16_t (T::*Method)(uint16_t)>
static int16_t pebble_callback_handler(struct ::MenuLayer *menu_layer, uint16_t section_index, void *callback_context)
{
    return (((T*)callback_context)->*Method)(section_index);
} 

template<class T, void (T::*Method)(const GContext&, const Layer&, const MenuIndex&)>
static void pebble_callback_handler(::GContext* ctx, const ::Layer *cell_layer, ::MenuIndex *cell_index, void *callback_context)
{
    typedef ::Layer* LayerPtr;
    (((T*)callback_context)->*Method)(ctx, Layer(const_cast<LayerPtr>(cell_layer)), *cell_index);
} 

template<class T, void (T::*Method)(const GContext&, const Layer&, uint16_t)>
static void pebble_callback_handler(::GContext* ctx, const ::Layer *cell_layer, uint16_t section_index, void *callback_context)
{
    typedef ::Layer* LayerPtr;
    (((T*)callback_context)->*Method)(ctx, Layer(const_cast<LayerPtr>(cell_layer)), section_index);
} 

template<class T, void (T::*Method)(const MenuIndex&)>
static void pebble_callback_handler(::MenuLayer *menu_layer, ::MenuIndex *cell_index, void *callback_context)
{
    (((T*)callback_context)->*Method)(*cell_index);
} 

template<class T, void (T::*Method)(const MenuIndex&, const MenuIndex&)>
static void pebble_callback_handler(::MenuLayer *menu_layer, ::MenuIndex new_index, ::MenuIndex old_index, void *callback_context)
{
    (((T*)callback_context)->*Method)(new_index, old_index);
} 

template<class T, void (T::*Method)(MenuIndex&, const MenuIndex&)>
static void pebble_callback_handler(::MenuLayer *menu_layer, ::MenuIndex *new_index, ::MenuIndex old_index, void *callback_context)
{
    (((T*)callback_context)->*Method)(*new_index, old_index);
} 

template<class T, void (T::*Method)(const GContext&, const Layer&, bool)>
static void pebble_callback_handler(::GContext* ctx, const ::Layer *bg_layer, bool highlight, void *callback_context)
{
    typedef ::Layer* LayerPtr;
    (((T*)callback_context)->*Method)(ctx, Layer(const_cast<LayerPtr>(bg_layer)), highlight);
} 
// ---------------------------------------


class MenuLayer : public Layer
{
   
public:
    MenuLayer(GRect frame) : Layer((menu_layer_ = menu_layer_create(frame), menu_layer_get_layer(menu_layer_))) { }
    
    MenuLayer() : MenuLayer(GRect(0, 0, 0, 0)) {}
     
    //! Destroys the underlying TextLayer
    virtual ~MenuLayer() {
        menu_layer_destroy(menu_layer_);
    }
    
    //! Instances of this class can be automatically converted to Pebble TextLayers
    inline operator ::MenuLayer*() { return menu_layer_; }

    inline void SetClickConfigOntoWindow(Window& owner_window) { menu_layer_set_click_config_onto_window(menu_layer_, owner_window); }

    inline void SetSelectedNext(MenuRowAlign scroll_align, bool animated) { menu_layer_set_selected_next(menu_layer_, false, scroll_align, animated); }

    inline void SetSelectedPrevious(MenuRowAlign scroll_align, bool animated) { menu_layer_set_selected_next(menu_layer_, true, scroll_align, animated); }

    inline void SetSelectedIndex(MenuIndex index, MenuRowAlign scroll_align, bool animated) { menu_layer_set_selected_index(menu_layer_, index, scroll_align, animated); }

    inline void SetSelectedIndex(uint16_t section, uint16_t row, MenuRowAlign scroll_align, bool animated) 
    { 
        SetSelectedIndex({ .section = section, .row = row }, scroll_align, animated);
    }

    inline MenuIndex GetSelectedIndex() const { return menu_layer_get_selected_index(menu_layer_); }

    inline void ReloadData() { menu_layer_reload_data(menu_layer_); }    

    inline void SetNormalColors(GColor background, GColor foreground) { menu_layer_set_normal_colors(menu_layer_, background, foreground); }

    inline void SetHighlightColors(GColor background, GColor foreground) { menu_layer_set_highlight_colors(menu_layer_, background, foreground); }

    inline void PadBottom(bool enable) { menu_layer_pad_bottom_enable(menu_layer_, enable); }

    inline bool GetCenterFocused() const { return menu_layer_get_center_focused(menu_layer_); }

    inline void SetCenterFocused(bool center_focused) { menu_layer_set_center_focused(menu_layer_, center_focused); }

    inline bool IsIndexSelected(const MenuIndex& index) const { return menu_layer_is_index_selected(menu_layer_, const_cast<MenuIndex*>(&index)); }

    inline bool IsIndexSelected(uint16_t section, uint16_t row) const 
    {
        return IsIndexSelected({ .section = section, .row = row });       
    }
    
    template<class T>
    inline void SetCallbacks(T& owner, MenuLayerCallbacks callbacks)
    {
        menu_layer_set_callbacks(menu_layer_, &owner, callbacks);
    }

    static bool IsHighlighted(const Layer& cell_layer) { return menu_cell_layer_is_highlighted(cell_layer); }

    static int16_t IndexCompare(const MenuIndex& a, const MenuIndex& b) { return menu_index_compare(&a, &b); }

protected:
    // The layer pointer
    ::MenuLayer* menu_layer_;
};


class MenuEvents
{
protected:
    //! Enable the events for the menu.  This can be called anytime but should be called when the events
    //! will return correct data.  If the data changes, you must call ReloadData()
    void EnableMenuEvents(MenuLayer& menuLayer, bool hasHeaders, bool hasSeparators, bool longClicks, bool customBackground)
    {
        menuLayer.SetCallbacks(*this, (MenuLayerCallbacks){
            .get_num_sections = CALLBACK(OnGetNumSections),
            .get_num_rows = CALLBACK(OnGetNumRows),
            .get_cell_height = CALLBACK(OnGetCellHeight),
            .get_header_height = hasHeaders ? CALLBACK(OnGetHeaderHeight) : nullptr,
            .draw_row = CALLBACK(OnDrawRow),
            .draw_header = hasHeaders ? CALLBACK(OnDrawHeader) : nullptr,
            .select_click = CALLBACK(OnSelectClick),
            .select_long_click = longClicks ? CALLBACK(OnSelectLongClick) : nullptr,
            .selection_changed = CALLBACK(OnSelectionChanged),
            .get_separator_height =  hasSeparators ? CALLBACK(OnGetSeparatorHeight) : nullptr,
            .draw_separator = hasSeparators ? CALLBACK(OnDrawSeparator) : nullptr,
            .selection_will_change = CALLBACK(OnSelectionWillChange),
            .draw_background = customBackground ? CALLBACK(OnDrawBackground) : nullptr
        });        
    }

    //! Event to get the number of sections in a menu.
    //! @return The number of sections in the menu
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual uint16_t OnGetNumSections() { return 1; }

    //! Event to get the number of rows in a given section in a menu.
    //! @param section_index The index of the section of the menu for which the
    //! number of items it contains is requested
    //! @return The number of rows in the given section in the menu
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual uint16_t OnGetNumRows(uint16_t section_index) = 0;

    //! Function signature for the callback to get the height of the menu cell
    //! at a given index.
    //! @param menu_layer The \ref MenuLayer for which the data is requested
    //! @param cell_index The MenuIndex for which the cell height is requested
    //! @param callback_context The callback context
    //! @return The height of the cell at the given MenuIndex
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual int16_t OnGetCellHeight(const MenuIndex& cell_index) {
        return 44; // MENU_CELL_BASIC_CELL_HEIGHT
    }

    //! Event to get the height of the section header at a given section index.
    //! @param section_index The index of the section for which the header height is
    //! requested
    //! @return The height of the section header at the given section index
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual int16_t OnGetHeaderHeight(uint16_t section_index)
    {
        return 0;
    }

    //! Function signature for the callback to get the height of the separator
    //! at a given index.
    //! @param menu_layer The \ref MenuLayer for which the data is requested
    //! @param cell_index The MenuIndex for which the cell height is requested
    //! @param callback_context The callback context
    //! @return The height of the separator at the given MenuIndex
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual int16_t OnGetSeparatorHeight(const MenuIndex& cell_index)
    {
        return 0;
    }

    //! Function signature for the callback to render the menu cell at a given
    //! MenuIndex.
    //! @param ctx The destination graphics context to draw into
    //! @param cell_layer The cell's layer, containing the geometry of the cell
    //! @param cell_index The MenuIndex of the cell that needs to be drawn
    //! @note The `cell_layer` argument is provided to make it easy to re-use an
    //! `.update_proc` implementation in this callback. Only the bounds and frame
    //! of the `cell_layer` are actually valid and other properties should be
    //! ignored.
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnDrawRow(const GContext& ctx, const Layer& cell_layer, const MenuIndex& cell_index) = 0;

    //! Function signature for the callback to render the section header at a given
    //! @param ctx The destination graphics context to draw into
    //! @param cell_layer The header cell's layer, containing the geometry of the
    //! header cell
    //! @param section_index The section index of the section header that needs to
    //! be drawn
    //! @note The `cell_layer` argument is provided to make it easy to re-use an
    //! `.update_proc` implementation in this callback. Only the bounds and frame
    //! of the `cell_layer` are actually valid and other properties should be
    //! ignored.
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnDrawHeader(const GContext& ctx, const Layer& cell_layer, uint16_t section_index) { }

    //! Function signature for the callback to render the separator at a given
    //! MenuIndex.
    //! @param ctx The destination graphics context to draw into
    //! @param cell_layer The cell's layer, containing the geometry of the cell
    //! @param cell_index The MenuIndex of the separator that needs to be drawn
    //! @param callback_context The callback context
    //! @note The `cell_layer` argument is provided to make it easy to re-use an
    //! `.update_proc` implementation in this callback. Only the bounds and frame
    //! of the `cell_layer` are actually valid and other properties should be
    //! ignored.
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnDrawSeparator(const GContext& ctx, const Layer& cell_layer, const MenuIndex& cell_index) { }

    //! Function signature for the callback to handle the event that a user hits
    //! the SELECT button.
    //! @param cell_index The MenuIndex of the cell that is selected
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnSelectClick(const MenuIndex& cell_index) { }

    //! Function signature for the callback to handle the event that a user hits
    //! the SELECT button.
    //! @param cell_index The MenuIndex of the cell that is selected
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnSelectLongClick(const MenuIndex& cell_index) { }

    //! Function signature for the callback to handle a change in the current
    //! selected item in the menu.
    //! @param new_index The MenuIndex of the new item that is selected now
    //! @param old_index The MenuIndex of the old item that was selected before
    //! @see \ref menu_layer_set_callbacks()
    //! @see \ref MenuLayerCallbacks
    virtual void OnSelectionChanged(const MenuIndex& new_index, const MenuIndex& old_index) { }

    //! Function signature for the callback which allows or changes selection behavior of the menu.
    //! In order to change the cell that should be selected, modify the passed in new_index.
    //! Preventing the selection from changing, new_index can be assigned the value of old_index.
    //! @param menu_layer The \ref MenuLayer for which the selection event that occured
    //! @param new_index Pointer to the index that the MenuLayer is going to change selection to.
    //! @param old_index The index that is being unselected.
    //! @param callback_context The callback context
    //! @note \ref menu_layer_set_selected_index will not trigger this callback when
    //! the selection changes, but \ref menu_layer_set_selected_next will.
    virtual void OnSelectionWillChange(MenuIndex& new_index, const MenuIndex& old_index) { }

    //! Function signature for the callback which draws the menu's background.
    //! The background is underneath the cells of the menu, and is visible in the
    //! padding below the bottom cell, or if a cell's background color is set to \ref GColorClear.
    //! @param ctx The destination graphics context to draw into.
    //! @param bg_layer The background's layer, containing the geometry of the background.
    //! @param highlight Whether this should be rendered as highlighted or not. Highlight style
    //! should match the highlight style of cells, since this color can be used for animating selection.
    virtual void OnDrawBackground(const GContext& ctx, const Layer& bg_layer, bool highlight) { }
};

}