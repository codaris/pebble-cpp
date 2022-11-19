#pragma once

namespace Pebble {

//! @addtogroup Clicks
//! \brief Handling button click interactions
//!
//! Each Pebble window handles Pebble's buttons while it is displayed. Raw button down and button
//! up events are transformed into click events that can be transferred to your app:
//!
//! * Single-click. Detects a single click, that is, a button down event followed by a button up event.
//! It also offers hold-to-repeat functionality (repeated click).
//! * Multi-click. Detects double-clicking, triple-clicking and other arbitrary click counts.
//! It can fire its event handler on all of the matched clicks, or just the last.
//! * Long-click. Detects long clicks, that is, press-and-hold.
//! * Raw. Simply forwards the raw button events. It is provided as a way to use both the higher level
//! "clicks" processing and the raw button events at the same time.
//!
//! To receive click events when a window is displayed, you must register a \ref ClickConfigProvider for
//! this window with \ref AppWindow::SetClickConfigProvider(). Your \ref ClickConfigProvider will be called every time
//! the window becomes visible with one context argument. 
//!
//! In your \ref ClickConfigProvider you call the \ref window_single_click_subscribe(), \ref window_single_repeating_click_subscribe(),
//! \ref window_multi_click_subscribe(), \ref window_long_click_subscribe() and \ref window_raw_click_subscribe() functions to register
//! a handler for each event you wish to receive.
//!
//! For convenience, click handlers are provided with a \ref ClickRecognizerRef and a user-specified context.
//!
//! The \ref ClickRecognizerRef can be used in combination with \ref click_number_of_clicks_counted(), \ref
//! click_recognizer_get_button_id() and \ref click_recognizer_is_repeating() to get more information about the click. This is
//! useful if you want different buttons or event types to share the same handler.
//!
//! The user-specified context is the context of your \ref ClickConfigProvider (see above). By default it points to the window.
//! You can override it for all handlers with \ref window_set_click_config_provider_with_context() or for a specific button with \ref
//! window_set_click_context().
//!
//! <h3>User interaction in watchfaces</h3>
//! Watchfaces cannot use the buttons to interact with the user. Instead, you can use the \ref AccelerometerService.
//!
//! <h3>About the Back button</h3>
//! By default, the Back button will always pop to the previous window on the \ref WindowStack (and leave the app if the current
//! window is the only window). You can override the default back button behavior with \ref window_single_click_subscribe() and
//! \ref window_multi_click_subscribe() but you cannot set a repeating, long or raw click handler on the back button because a long press
//! will always terminate the app and return to the main menu.
//!
//! <h3>See also</h3>
//! Refer to the \htmlinclude UiFramework.html (chapter "Clicks") for a conceptual
//! overview of clicks and relevant code examples.
//!
//! @{

//! Reference to opaque click recognizer
//! When a \ref ClickHandler callback is called, the recognizer that fired the handler is passed in.
//! @see \ref ClickHandler
//! @see \ref NumberOfClicksCounted()
//! @see \ref GetButtonId()
//! @see \ref IsRepeating()
class ClickRecognizer
{
public:
    //! Constructs a ClickRecognizer instance
    //! @param recognizer Pebble recognizer to wrap
    ClickRecognizer(::ClickRecognizerRef recognizer) : recognizer_(recognizer) {}

    //! Instances of this class can be automatically converted to Pebble ClickRecognizerRef
    inline operator ::ClickRecognizerRef() { return recognizer_; } 
  
    //! Gets the click count.
    //! You can use this inside a click handler implementation to get the click count for multi_click
    //! and (repeated) click events.
    //! @return The number of consecutive clicks, and for auto-repeating the number of repetitions.
    inline uint8_t NumberOfClicksCounted() { return click_number_of_clicks_counted(recognizer_); }
    
    //! Gets the button identifier.
    //! You can use this inside a click handler implementation to get the button id for the click event.
    //! @return the ButtonId of the click recognizer    
    inline ButtonId GetButtonId() { return click_recognizer_get_button_id(recognizer_); }

    //! Is this a repeating click.
    //! You can use this inside a click handler implementation to find out whether this is a repeating click or not.
    //! @return true if this is a repeating click.
    inline bool IsRepeating() { return click_recognizer_is_repeating(recognizer_); }
    
protected:
    // The underlying recognizer
    ::ClickRecognizerRef recognizer_;
    
};

class ClickConfig
{
public:
    //! Subscribe to long click events.
    //! @note Must be called from the \ref ClickConfigProvider.
    //! @note The back button cannot be overridden with a long click.
    //! @param button_id The button events to subscribe to.
    //! @param delay_ms Milliseconds after which "handler" is fired. A value of 0 means to use the system default 500ms.
    //! @param down_handler The \ref ClickHandler to fire as soon as the button has been held for `delay_ms`. This may be NULL to have no down handler.
    //! @param up_handler The \ref ClickHandler to fire on the release of a long click. This may be NULL to have no up handler.
    inline void LongClick(ButtonId button_id, uint16_t delay_ms, ::ClickHandler down_handler, ::ClickHandler up_handler) const
    {
        window_long_click_subscribe(button_id, delay_ms, down_handler, up_handler);
    }

    //! Subscribe to multi click events.
    //! @note Must be called from the \ref ClickConfigProvider.
    //! @param button_id The button events to subscribe to.
    //! @param min_clicks Minimum number of clicks before handler is fired. Defaults to 2.
    //! @param max_clicks Maximum number of clicks after which the click counter is reset. A value of 0 means use "min" also as "max".
    //! @param timeout The delay after which a sequence of clicks is considered finished, and the click counter is reset. A value of 0 means to use the system default 300ms.
    //! @param last_click_only Defaults to false. When true, only the handler for the last multi-click is called.
    //! @param handler The \ref ClickHandler to fire on this event. Fired for multi-clicks, as "filtered" by the `last_click_only`, `min`, and `max` parameters.
    inline void MultiClick(ButtonId button_id, uint8_t min_clicks, uint8_t max_clicks, uint16_t timeout, bool last_click_only, ::ClickHandler handler)  const
    {
        window_multi_click_subscribe(button_id, min_clicks, max_clicks, timeout, last_click_only, handler);
    }

    //! Subscribe to raw click events.
    //! @note Must be called from within the \ref ClickConfigProvider.
    //! @note The back button cannot be overridden with a raw click.
    //! @param button_id The button events to subscribe to.
    //! @param down_handler The \ref ClickHandler to fire as soon as the button has been pressed. This may be NULL to have no down handler.
    //! @param up_handler The \ref ClickHandler to fire on the release of the button. This may be NULL to have no up handler.
    //! @param context If this context is not NULL, it will override the general context.
    inline void RawClick(ButtonId button_id, ::ClickHandler down_handler, ::ClickHandler up_handler)  const
    {
        window_raw_click_subscribe(button_id, down_handler, up_handler, NULL);
    }
    
    //! Subscribe to single click events.
    //! @note Must be called from the \ref ClickConfigProvider.
    //! @note \ref window_single_click_subscribe() and \ref window_single_repeating_click_subscribe() conflict, and cannot both be used on the same button.
    //! @note When there is a multi_click and/or long_click setup, there will be a delay before the single click
    //! @param button_id The button events to subscribe to.
    //! @param handler The \ref ClickHandler to fire on this event.
    //! handler will get fired. On the other hand, when there is no multi_click nor long_click setup, the single click handler will fire directly on button down.
    //! @see ButtonId
    //! @see Clicks
    //! @see SingleRepeatingClick
    inline void SingleClick(ButtonId button_id, ::ClickHandler handler) const
    {
        window_single_click_subscribe(button_id, handler);
    }
    
    //! Subscribe to single click event, with a repeat interval. A single click is detected every time "repeat_interval_ms" has been reached.
    //! @note Must be called from the \ref ClickConfigProvider.
    //! @note \ref window_single_click_subscribe() and \ref window_single_repeating_click_subscribe() conflict, and cannot both be used on the same button.
    //! @note The back button cannot be overridden with a repeating click.
    //! @param button_id The button events to subscribe to.
    //! @param repeat_interval_ms When holding down, how many milliseconds before the handler is fired again.
    //! A value of 0ms means "no repeat timer". The minimum is 30ms, and values below will be disregarded.
    //! If there is a long-click handler subscribed on this button, `repeat_interval_ms` will not be used.
    //! @param handler The \ref ClickHandler to fire on this event.
    //! @see SingleClick
    inline void SingleRepeatingClick(ButtonId button_id, uint16_t repeat_interval_ms, ::ClickHandler handler) const
    {
         window_single_repeating_click_subscribe(button_id, repeat_interval_ms, handler);
    }
};

//! @}

}