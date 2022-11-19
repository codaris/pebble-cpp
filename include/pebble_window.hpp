#pragma once

namespace Pebble {

// Window callback handler
template<class T, void (T::*Method)(void)>
static void pebble_callback_handler(::Window* window);

// Click conflig callback handler
template<class T, void (T::*Method)(const ClickConfig&)>
static void pebble_callback_handler(void* context);

// Click callback handler
template<class T, void (T::*Method)(const ClickRecognizer&)>
static void pebble_callback_handler(::ClickRecognizerRef recognizer, void* context);

// Numberwindow callback handler
template<class T, void (T::*Method)(void)>
static void pebble_callback_handler(::NumberWindow* window, void* context);


//! @addtogroup Window
//! @{

//! Wraps and existing Pebble window but does not take ownership of it.    
class Window : public Layer
{
public:
    //! Constructs a Window instance
    //! @param Existing Pebble window to wrap
    Window(::Window* window) : Layer(window_get_root_layer(window)), window_(window)
    {
    }

    //! Window destructor    
    virtual ~Window() 
    {
        // Does not destroy the Pebble window has this base class does not take ownership.
    }
    
    //! Instances of this class can be automatically converted to Pebble window pointers
    operator ::Window*() { return window_; }

    //! Pushes this window onto the window navigation stack,
    //! on top of the current topmost window of the app.
    //! @param animated Pass in `true` to animate the push using a sliding animation,
    //! or `false` to skip the animation.
    inline void StackPush(bool animated) { window_stack_push(window_, animated); }
    
    //! Checks if the window is on the window stack
    //! @return true if the window is currently on the window stack.    
    inline bool StackContainsWindow() const { return window_stack_contains_window(window_); }
    
    //! Returns whether or not this Window is on the top of the app's window stack.
    //! @return True if the topmost window on the stack that belongs to the app is this window.
    inline bool WindowIsTop() { return window_ == window_stack_get_top_window(); }
    
    //! Removes a given window from the window stack
    //! that belongs to the app task.
    //! @note If there are no windows for the app left on the stack, the app
    //! will be killed by the system, shortly. To avoid this, make sure
    //! to push another window shortly after or before removing the last window.
    //! @param animated Pass in `true` to animate the removal of the window using
    //! a side-to-side sliding animation to reveal the next window.
    //! This is only used in case the window happens to be on top of the window
    //! stack (thus visible).
    //! @return True if window was successfully removed, false otherwise.    
    inline bool StackRemove(bool animated) { return window_stack_remove(window_, animated); } 

    //! Gets whether the window has been loaded.
    //! If a window is loaded, its `.load` handler has been called (and the `.unload` handler
    //! has not been called since).
    //! @return true if the window is currently loaded or false if not.
    //! @see \ref WindowHandlers    
    inline bool IsLoaded() const { return window_is_loaded(window_); }

    //! Gets the current click configuration provider of the window.
    inline ::ClickConfigProvider GetClickConfigProvider() const { return window_get_click_config_provider(window_); }

    //! Sets the background color of the window, which is drawn automatically by the
    //! root layer of the window.
    //! @param background_color The new background color
    //! @see \ref window_get_root_layer()    
    inline void SetBackgroundColor(GColor background_color) { window_set_background_color(window_, background_color); }

protected:
    // Private instance of window pebble Window pointer
    ::Window *window_;

}; // End Class


//! Base class for application windows
class AppWindow : public Window
{
public:
    //! Constructs a new Window and initalizes it with the default values.  This constructor 
    //! also sets up all the window events.
    AppWindow() : Window(window_create())
    { 
        window_set_user_data(window_, this); 
        window_set_window_handlers(window_, {
            .load = CALLBACK(OnLoad),
            .appear = CALLBACK(OnAppear),
            .disappear = CALLBACK(OnDisappear),
            .unload = CALLBACK(OnUnload)
        });
    }
    
    //! Destroys the underlying Pebble window
    virtual ~AppWindow() 
    {
        // The assignment to a static variable makes the virtual methods work for subclasses
        // I have no idea why!
        dummy() = 0; 
        window_destroy(window_);    
    }
      
    //! Returns an object of the template class type from the specified window's user data.
    template<class T>
    static inline T& Get(::Window* window) { return *(T*)(window_get_user_data(window)); } 
      
protected:
    //! OnLoad handler:  called when the window is pushed to the screen when it's not loaded. 
    //! This is a good moment to do the layout of the window.
    virtual void OnLoad() { }; 

    //! OnUnload handler: called when the window is deinited, but could be used in the future 
    //! to free resources bound to windows that are not on screen.
    virtual void OnUnload() { }; 

    //! OnAppear handler: called when the window comes on the screen (again). 
    //! E.g. when second-top-most window gets revealed (again) after popping the top-most window, 
    //! but also when the window is pushed for the first time. This is a good moment to start timers 
    //! related to the window, or reset the UI, etc.
    virtual void OnAppear() { }; 

    //! OnDisappear handler: called when the window leaves the screen, e.g. when another window is pushed, 
    //! or this window is popped. Good moment to stop timers related to the window.
    virtual void OnDisappear() { }; 

    //! Sets the click config provider of this window to a local method
    inline void SetClickConfigProvider(::ClickConfigProvider click_config_provider)
    {
        window_set_click_config_provider_with_context(window_, click_config_provider, this);
    }

private:
    // Dummy static variable needed for virutal method pointers to work (see destructor)
    inline static int& dummy() { static int dummy = 0; return dummy; }  

}; // End Class


class NumberWindow : public Window
{
public:
    //! Constructs a new Window and initalizes it with the default values.  This constructor 
    //! also sets up all the window events.
    NumberWindow(const char* label) : Window((::Window*)number_window_create(
            label, 
            {
                .incremented = CALLBACK(OnIncremented),
                .decremented = CALLBACK(OnDecremented),
                .selected = CALLBACK(OnSelected),
            },
            this
        ))
    { 
    }
 
    //! Destroys the underlying Pebble window
    virtual ~NumberWindow() {
        number_window_destroy((::NumberWindow*)window_);    
    }
    
    operator ::NumberWindow*() { return (::NumberWindow*)window_; }    
    
    inline int32_t GetValue() const { return number_window_get_value((::NumberWindow*)window_); }
    
    inline void SetValue(int32_t value) { number_window_set_value((::NumberWindow*)window_, value); }
    
    inline void SetLabel(const char* label) { number_window_set_label((::NumberWindow*)window_, label); }
    
    inline void SetMin(int32_t min) { number_window_set_min((::NumberWindow*)window_, min); }

    inline void SetMax(int32_t max) { number_window_set_max((::NumberWindow*)window_, max); }
    
    inline void SetStepSize(int32_t step) { number_window_set_step_size((::NumberWindow*)window_, step); }

protected:
    virtual void OnIncremented() { }; 

    virtual void OnDecremented() { }; 

    virtual void OnSelected() { }; 
  
}; // End Class

// Window callback handler implementation
template<class T, void (T::*Method)(void)>
static void pebble_callback_handler(::Window* window)
{
    (AppWindow::Get<T>(window).*Method)();
}     

// Click configuration handler implementation
template<class T, void (T::*Method)(const ClickConfig&)>
static void pebble_callback_handler(void* context)
{
    (((T*)context)->*Method)(ClickConfig());
}       

// Click handler implementation
template<class T, void (T::*Method)(const ClickRecognizer&)>
static void pebble_callback_handler(::ClickRecognizerRef recognizer, void* context)
{
    (((T*)context)->*Method)(ClickRecognizer(recognizer));
}   

// Number window handler implementation
template<class T, void (T::*Method)(void)>
static void pebble_callback_handler(::NumberWindow* window, void* context)
{
    (((T*)context)->*Method)();
}   


//! @} // group Window

}
