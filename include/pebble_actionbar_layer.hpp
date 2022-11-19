#pragma once

#include "pebble.hpp"

namespace Pebble {

//! @addtogroup ActionBarLayer
//! @{

//! Vertical, bar-shaped control widget on the right edge of the window
class ActionBarLayer : public Layer
{
   
public:
    //! Creates a new ActionBarLayer and initalizes it with the default values.
    //! * Background color: \ref GColorBlack
    //! * No click configuration provider (`NULL`)
    //! * No icons
    //! * Not added to / associated with any window, thus not catching any button input yet.
    ActionBarLayer() : Layer((actionbar_layer_ =  action_bar_layer_create(), action_bar_layer_get_layer(actionbar_layer_))) { }
     
    //! Destroys the underlying Pebble ActionBarLayer
    virtual ~ActionBarLayer() 
    {
        action_bar_layer_destroy(actionbar_layer_);
    }
    
    //! Instances of this class can be automatically converted to Pebble TextLayers
    inline operator ::ActionBarLayer*() { return actionbar_layer_; }

    //! Sets the click configuration provider callback of the action bar.
    //! @note If the action bar had already been added to a window and the window
    //! is currently on-screen, the click configuration provider will be called
    //! before this function returns. Otherwise, it will be called by the system
    //! when the window becomes on-screen.
    //! @note The `.raw` handlers cannot be used without breaking the automatic
    //! highlighting of the segment of the action bar that for which a button is
    //! @param owner The owner of the click configuration provider method
    //! @param click_config_provider The new click configuration provider
    template<class T>
    inline void SetClickConfigProvider(T& owner, ::ClickConfigProvider click_config_provider) 
    {
        action_bar_layer_set_context(actionbar_layer_, &owner);        
        action_bar_layer_set_click_config_provider(actionbar_layer_, click_config_provider);
    }

    //! Sets an action bar icon onto one of the 3 slots as identified by `button_id`.
    //! Only \ref BUTTON_ID_UP, \ref BUTTON_ID_SELECT and \ref BUTTON_ID_DOWN can be
    //! used. Optionally, if `animated` is `true`, the transition will be animated.
    //! Whenever an icon is set, the click configuration provider will be called,
    //! to give the application the opportunity to reconfigure the button interaction.
    //! @param button_id The identifier of the button for which to set the icon
    //! @param icon Pointer to the \ref GBitmap icon
    //! @param animated True = animate the transition, False = do not animate the transition
    inline void SetIcon(ButtonId button_id, const GBitmap& icon, bool animated = false) 
    {
        action_bar_layer_set_icon_animated(actionbar_layer_, button_id, icon, animated);
    }

    //! Clears out an existing icon.
    //! @param button_id The identifier of the button for which to clear the icon
    inline void ClearIcon(ButtonId button_id) { action_bar_layer_clear_icon(actionbar_layer_, button_id); }

    //! Adds the action bar's layer on top of the window's root layer. It also
    //! adjusts the layout of the action bar to match the geometry of the window it
    //! gets added to.
    //! Lastly, it calls \ref window_set_click_config_provider_with_context() on
    //! the window to set it up to work with the internal callback and raw click
    //! handlers of the action bar, to enable the highlighting of the section of the
    //! action bar when the user presses a button.
    //! @note After this call, do not use
    //! \ref AppWindow::SetClickConfigProvider() with the window that
    //! the action bar has been added to (this would de-associate the action bar's
    //! click config provider and context). Instead use
    //! \ref SetClickConfigProvider() to register the click configuration
    //! provider to configure the buttons actions.
    //! @note It is advised to call this is in the window's `.load` or `.appear`
    //! handler. Make sure to call \ref RemoveFromWindow() in the
    //! window's `.unload` or `.disappear` handler.
    //! @note Adding additional layers to the window's root layer after this calll
    //! can occlude the action bar.
    //! @param window The window with which the action bar is to be associated
    inline void AddToWindow(Window& window) { action_bar_layer_add_to_window(actionbar_layer_, window); }

    //! Removes the action bar from the window and unconfigures the window's
    //! click configuration provider. `NULL` is set as the window's new click config
    //! provider and also as its callback context. If it has not been added to a
    //! window before, this function is a no-op.
    inline void RemoveFromWindow() { action_bar_layer_remove_from_window(actionbar_layer_); }

    //! Sets the animation to use while a button is pressed on an ActionBarLayer.
    //! By default we use ActionBarLayerIconPressAnimationMoveLeft
    //! @param button_id The button for which to set the press animation
    //! @param animation The animation to use.
    //! @see SetIcon()
    //! @see SetClickConfigProvider()
    inline void SetIconPressAnimation(ButtonId button_id, ActionBarLayerIconPressAnimation animation) 
    {
        action_bar_layer_set_icon_press_animation(actionbar_layer_, button_id, animation);
    }

protected:
    // The layer pointer
    ::ActionBarLayer* actionbar_layer_;
};

//! @} // group ActionBarLayer

}