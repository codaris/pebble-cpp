#pragma once

namespace Pebble {

//! @addtogroup Layer Layers
//! \brief User interface layers for displaying graphic components
//!
//! Layers are objects that can be displayed on a Pebble watchapp window, enabling users to see
//! visual objects, like text or images. Each layer stores the information about its state
//! necessary to draw or redraw the object that it represents and uses graphics routines along with
//! this state to draw itself when asked. Layers can be used to display various graphics.
//!
//! Layers are the basic building blocks for your application UI. Layers can be nested inside each other.
//! Every window has a root layer which is always the topmost layer.
//! You provide a function that is called to draw the content of the layer when needed; or
//! you can use standard layers that are provided by the system, such as text layer, image layer,
//! menu layer, action bar layer, and so on.
//!
//! The Pebble layer hierarchy is the list of things that need to be drawn to the screen.
//! Multiple layers can be arranged into a hierarchy. This enables ordering (front to back),
//! layout and hierarchy. Through relative positioning, visual objects that are grouped together by
//! adding them into the same layer can be moved all at once. This means that the child layers
//! will move accordingly. If a parent layer has clipping enabled, all the children will be clipped
//! to the frame of the parent.
//!
//! Pebble OS provides convenience layers with built-in logic for displaying different graphic
//! components, like text and bitmap layers.
//!
//! Refer to the \htmlinclude UiFramework.html (chapter "Layers") for a conceptual overview
//! of Layers and relevant code examples.
//!
//! @{

class Layer
{
public:
    //! Wraps an existing TextLayer
    Layer(::Layer *layer) : layer_(layer) {};    

    virtual ~Layer() { }
    
    //! Instances of this class can be automatically converted to Pebble Layers
    operator ::Layer*() { return layer_; }
    operator const ::Layer*() const { return layer_; }
    
    //! Marks the complete layer as "dirty", awaiting to be asked by the system to redraw itself.
    //! Typically, this function is called whenever state has changed that affects what the layer
    //! is displaying.
    //! * The layer's `.update_proc` will not be called before this function returns,
    //! but will be called asynchronously, shortly.
    //! * Internally, a call to this function will schedule a re-render of the window that the
    //! layer belongs to. In effect, all layers in that window's layer hierarchy will be asked to redraw.
    //! * If an earlier re-render request is still pending, this function is a no-op.
    inline void MarkDirty() { layer_mark_dirty(layer_); }

    //! Sets the frame of the layer, which is it's bounding box relative to the coordinate
    //! system of its parent layer.
    //! The size of the layer's bounds will be extended automatically, so that the bounds
    //! cover the new frame.
    //! @param frame The new frame
    //! @see \ref layer_set_bounds()
    inline void SetFrame(GRect frame) { layer_set_frame(layer_, frame); };
    inline void SetFrame(int16_t x, int16_t y, int16_t w, int16_t h) { SetFrame(GRect(x, y, w, h)); }
    inline void SetFrame(GPoint origin,  GSize size) { SetFrame(GRect(origin, size)); }

    //! Gets the frame of the layer, which is it's bounding box relative to the coordinate
    //! system of its parent layer.
    //! If the frame has changed, \ref layer_mark_dirty() will be called automatically.
    //! @return The frame of the layer
    //! @see layer_set_frame
    inline GRect GetFrame() { return GRect(layer_get_frame(layer_)); }

    //! Sets the bounds of the layer, which is it's bounding box relative to its frame.
    //! If the bounds has changed, \ref layer_mark_dirty() will be called automatically.
    //! @param bounds The new bounds
    //! @see \ref layer_set_frame()
    inline void SetBounds(GRect bounds) { layer_set_bounds(layer_, bounds); }

    //! Gets the bounds of the layer
    //! @return The bounds of the layer
    //! @see layer_set_bounds
    inline GRect GetBounds() { return GRect(layer_get_bounds(layer_)); }

    //! Gets the window that the layer is currently attached to.
    //! @param layer The layer for which to get the window
    //! @return The window that this layer is currently attached to, or `NULL` if it has
    //! not been added to a window's layer hierarchy.
    //! @see \ref window_get_root_layer()
    //! @see \ref layer_add_child()
    //inline Window& GetWindow() { return Windolayer_get_window(const Layer *layer);

    //! Removes the layer from its current parent layer
    //! If removed successfully, the child's parent layer will be marked dirty
    //! automatically.
    //! @param child The layer to remove
    inline void RemoveFromParent() { layer_remove_from_parent(layer_); }

    //! Removes child layers from given layer
    //! If removed successfully, the child's parent layer will be marked dirty
    //! automatically.
    inline void RemoveChildLayers() { layer_remove_child_layers(layer_); }

    //! Adds the child layer to a given parent layer, making it appear
    //! in front of its parent and in front of any existing child layers
    //! of the parent.
    //! If the child layer was already part of a layer hierarchy, it will
    //! be removed from its old parent first.
    //! If added successfully, the parent (and children) will be marked dirty
    //! automatically.
    //! @param parent The layer to which to add the child layer
    //! @param child The layer to add to the parent layer
    inline void AddChild(Layer& child) { layer_add_child(layer_, child); }

    //! Inserts the layer as a sibling behind another layer.
    //! The below_layer has to be a child of a parent layer,
    //! otherwise this function will be a noop.
    //! If inserted successfully, the parent (and children) will be marked dirty
    //! automatically.
    //! @param layer_to_insert The layer to insert into the hierarchy
    //! @param below_sibling_layer The layer that will be used as the sibling layer
    //! above which the insertion will take place
    inline void InsertBelowSibling(Layer& below_sibling_layer) { layer_insert_below_sibling(layer_, below_sibling_layer); }

    //! Inserts the layer as a sibling in front of another layer.
    //! The above_layer has to be a child of a parent layer,
    //! otherwise this function will be a noop.
    //! If inserted successfully, the parent (and children) will be marked dirty
    //! automatically.
    //! @param layer_to_insert The layer to insert into the hierarchy
    //! @param above_sibling_layer The layer that will be used as the sibling layer
    //! below which the insertion will take place
    inline void InsertAboveSibling(Layer& above_sibling_layer) { layer_insert_above_sibling(layer_, above_sibling_layer); }

    //! Sets the visibility of the layer.
    //! If the visibility has changed, \ref layer_mark_dirty() will be called automatically
    //! on the parent layer.
    //! @param layer The layer for which to set the visibility
    //! @param hidden Supply `true` to make the layer hidden, or `false` to make it
    //! non-hidden.
    inline void SetHidden(bool hidden) { layer_set_hidden(layer_, hidden); }

    //! Gets the visibility of the layer.
    //! @param layer The layer for which to get the visibility
    //! @return True if the layer is hidden, false if it is not hidden.
    inline bool GetHidden() { return layer_get_hidden(layer_); }

    //! Sets whether clipping is enabled for the layer. If enabled, whatever the layer _and
    //! its children_ will draw using their `.update_proc` callbacks, will be clipped by the
    //! this layer's frame.
    //! If the clipping has changed, \ref layer_mark_dirty() will be called automatically.
    //! @param layer The layer for which to set the clipping property
    //! @param clips Supply `true` to make the layer clip to its frame, or `false`
    //! to make it non-clipping.
    inline void SetClips(bool clips) { return layer_set_clips(layer_, clips); }

    //! Gets whether clipping is enabled for the layer.  If enabled, whatever the layer _and
    //! its children_ will draw using their `.update_proc` callbacks, will be clipped by the
    //! this layer's frame.
    //! @param layer The layer for which to get the clipping property
    //! @return True if clipping is enabled for the layer, false if clipping is not enabled for
    //! the layer.
    inline bool GetClips() { return layer_get_clips(layer_); }

protected:
    // The layer pointer
    ::Layer *layer_; 
}; // End class

//! \brief Layer that is automatically managed.
//!
//! This subclass of layer creates and destroys the underlying Pebble OS layer structure automatically.
//! This type of layer should be used as a container for other layers but does not do any drawing on
//! it's own.
//! @see Layer
class SimpleLayer : public Layer
{
public:
    //! Creates a layer and sets its frame and bounds.
    //! Default values:
    //! * `bounds` : origin (0, 0) and a size equal to the frame that is passed in.
    //! * `clips` : `true`
    //! * `hidden` : `false`
    //! @param frame The frame at which the layer should be initialized.
    SimpleLayer(GRect frame) : Layer(layer_create_with_data(frame, sizeof(this))) { 
        // Sets the lawyer data as pointer to this instance
        *(SimpleLayer**)layer_get_data(layer_) = this;
    }

    //! Destroys the underlying AppLayer
    virtual ~SimpleLayer() {
        layer_destroy(layer_);    
    }

protected:
    //! Given a pebble layer returns reference to the layer class
    //! @param layer Pebble lawyer
    //! @return Layer class instance
    static inline SimpleLayer& GetFromData(::Layer* layer) { return **(SimpleLayer**)(layer_get_data(layer)); } 

}; // End class

//! \brief Layer that is automatically managed and contains an OnUpdate callback for drawing
//!
//! This subclass of layer creates and destroys the underlying Pebble OS layer structure automatically.
//! This type of layer must be sublcassed to provide an implementation for the OnUpdate callback method.
//! @see Layer
class CustomLayer : public SimpleLayer
{
public:
    //! Creates a layer and sets its frame and bounds.
    //! Default values:
    //! * `bounds` : origin (0, 0) and a size equal to the frame that is passed in.
    //! * `clips` : `true`
    //! * `hidden` : `false`
    //! @param frame The frame at which the layer should be initialized.
    CustomLayer(GRect frame) : SimpleLayer(frame) { 
        // Sets the layer update procedure to static method
        layer_set_update_proc(layer_, layer_update_proc);
    }

protected:
    //! The layer's render function. The system will call this method automatically when the layer needs to redraw.
    //! @param ctx The destination graphics context to draw into    
    virtual void OnUpdate(const GContext& ctx) = 0;

private:
    //! Layer update procedure that call the OnUpdate method on class instance
    static void layer_update_proc(::Layer* layer, ::GContext *ctx) {
        // Retrieves the layer object instance of layer pointer, casts to CustomLayer, and calls update method
        static_cast<CustomLayer&>(GetFromData(layer)).OnUpdate(ctx);
    }    
}; // End class

//! @} // group Layer

}