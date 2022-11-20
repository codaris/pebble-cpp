# Using this SDK {#using}

## Installing the Package

Add the Pebble Foundation Classes package to your project using the following command:

```
> pebble package install pebble-cpp
> pebble build
```

The build step will unpack the files in the package so when can reference them.

## Updating the wscript file

The `wscript` file located in the project root contains the instructions for the Pebble build system to compile the source files.  This file needs to be modified to add commands to build C++ `*.cpp` files.  

You can copy an updated version of this file from the package folder `node_modules/pebble-cpp/dist/include/pebble-cpp/wscript`.  Execute the following command in the root of your project to copy this file over the existing `wscript` file.

```
> cp node_modules/pebble-cpp/dist/include/pebble-cpp/wscript .
```

Alternatively you can view and download the updated `wscript` file from [here](https://raw.githubusercontent.com/codaris/pebble-cpp/main/include/wscript).

## Structure of Source files

In order for your C++ code to be compiled by the C++ compiler they must have the `.cpp` file extension.  You can freely mix C `.c` and C++ `.cpp` files in your projects.  Although it's recommended that all your files be `.cpp` files -- you have full access to all Pebble SDK C functions in C++ files.

### The entry point

In order for Pebble to be able to build your project, it must have a main() function.  In C++ files, this main function must be declared `extern "C"` so the full entry point looks like this:

~~~~~~~{.cpp}
extern "C" int main(void) 
{
    // Your initialization code here

    // Run the event loop
    app_event_loop();
}
~~~~~~~

### The <Pebble.hpp> header

In C++ files, you do not include the `<pebble.h>` header.  Instead you include the Pebble Foundation Classes header from the pebble-cpp package `<pebble-cpp/pebble.hpp>`.  This header also includes `<pebble.h>` automatically giving you full access to all the Pebble SDK functions.

Since all the classes are in the `Pebble` namespace you can add `using namespace Pebble;` to make all the types available without needing the `Pebble::` prefix.

~~~~~~~{.cpp}
#include <pebble-cpp/pebble.hpp>

using namespace Pebble;
~~~~~~~

## The AppWindow class

To create a window in your pebble application you should subclass the `AppWindow` class.  This class has 4 virtual methods that you can override to manage the state of the Window:

~~~~~~~{.cpp}
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
~~~~~~~

Every `AppWindow` subclass should at least override the `OnLoad` handler to setup the window and any layers contained within it.

## Using Callbacks

The Pebble C API uses callbacks to provide functionality.  In the C API, you provide a function pointer that gets called when the Pebble OS needs to signal your code.  In Pebble Foundation Classes, this works in a similar way except you can provide methods instead of functions and the context parameter is handled automatically.  This greatly simplies managing callbacks.

To use a callback in your classes, you need to use the `CALLBACK` macro.  This macro takes
either a single method name as a parameter and returns a callback function.  You use this macro whenever you need to generate a callback to a method:

See the following class for an example:

~~~~~~~{.cpp}
class MainWindow : public AppWindow
{
protected:
    void OnLoad()
    {
        SetClickConfigProvider(CALLBACK(OnClickConfig));
        textLayer.SetFrame(0, 55, 144, 50);
        textLayer.SetBackgroundColor(GColorClear);
        textLayer.SetTextColor(GColorBlack);
        textLayer.SetFont(FONT_KEY_BITHAM_42_BOLD);
        textLayer.SetTextAlignment(GTextAlignmentCenter);
        AddChild(textLayer);
    }        
    
    void OnClickConfig(const ClickConfig& clickConfig)
    { 
        clickConfig.SingleClick(BUTTON_ID_SELECT, CALLBACK(OnSelectClick));
        clickConfig.SingleClick(BUTTON_ID_UP, CALLBACK(OnUpClick));
        clickConfig.SingleClick(BUTTON_ID_DOWN, CALLBACK(OnDownClick));
    }    
    
    void OnSelectClick(const ClickRecognizer& recognizer)
    {
        textLayer.SetText("Select");
    }    

    void OnDownClick(const ClickRecognizer& recognizer)
    {
        textLayer.SetText("Down");
    }    

    void OnUpClick(const ClickRecognizer& recognizer)
    {
        textLayer.SetText("Up");
    }    

private:
    Pebble::TextLayer textLayer;   
};
~~~~~~~

This class demonstrates how the `CALLBACK` macro is used to set the `ClickConfigProvider` for the window.  It is used again in `OnClickConfig` method to set the various click event handlers.

The library automatically handles managing the context pointer so that these callbacks are routed to your window instance.  You must use the correct types for the callback methods or you will get a build error.

## The TickTimer class

The `TickTimer` class is a fundamental class for watch faces.  It was what provides the tick events for updating the time on the display.  This class is meant to be subclassed and provides an `OnTick` method that you need to override.  

See the following class for an example:

~~~~~~~{.cpp}
class MainWindow : public AppWindow, public TickTimer
{
protected:
    void OnLoad()
    {
        textLayer.SetFrame(0, 55, 144, 50);
        textLayer.SetBackgroundColor(GColorClear);
        textLayer.SetTextColor(GColorBlack);
        textLayer.SetFont(FONT_KEY_BITHAM_42_BOLD);
        textLayer.SetTextAlignment(GTextAlignmentCenter);
        AddChild(textLayer);
        TickTimerStart(MINUTE_UNIT);
        UpdateTime();
    }        
    
    void UpdateTime()
    {
        DateTime date = DateTime::Now();    
        // Use 24 hour format
        date.Format(buffer, sizeof("00:00"), "%H:%M");
        textLayer.SetText(buffer);
    }    
    
    void OnTick(tm &tick_time, TimeUnits units_changed)
    {
        UpdateTime();
    }    

private:
    char buffer[5];
    Pebble::TextLayer textLayer;   
};
~~~~~~~

This class creates a `TextLayer` to hold the time and then starts the timer.  Every minute, the OnTick() method is called and the time is updated on the display.
