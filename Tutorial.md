# Tutorial {#tutorial}

Use the following steps to create a new C++ application from scratch.  In this short tutorial we will create a simple application that tells the time and responds to clicks.

### 1. Create a new project

From the command line start a new project as usual:

```
> pebble new-project helloworld
> cd helloworld
```

### 2. Install the pebble-cpp package

Then use the `pebble package install` command to install the `pebble-cpp` package.  Execute a `pebble build` to initialize the package.

```
> pebble package install pebble-cpp
> pebble build
```

### 3. Overwite the wscript file

The `wscript` file located in the project root contains the instructions for the Pebble build system to compile the source files.  This file needs to be modified to add commands to build C++ `*.cpp` files.  

You can copy an updated version of this file from the package folder `node_modules/pebble-cpp/dist/include/pebble-cpp/wscript`.  Execute the following command in the root of your project to copy this file over the existing `wscript` file.

```
> cp node_modules/pebble-cpp/dist/include/pebble-cpp/wscript .
```

Alternatively you can view and download the updated `wscript` file from [here](https://raw.githubusercontent.com/codaris/pebble-cpp/main/include/wscript).

### 4. Create a new C++ file

We've now added the ability to build C++ files but we haven't changed the ability to build C files.  In fact, you can freely mix C and C++ files.  But for the purposes of this tutorial we will erase the default C file and create a new single-file C++ application:

Remove the existing source file:
```
> cd src/c
> rm helloworld.c
```

And now in your favorite text editor, create a new file called `helloworld.cpp` in the  `src/c` folder.  In that file add the following contents:

~~~~~~~~~~~~~~~{.cpp}
#include <pebble-cpp/pebble.hpp>

using namespace Pebble;

class MainWindow : public AppWindow, public TickTimer
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
    char buffer[5];
    Pebble::TextLayer textLayer;   
};

extern "C" int main(void) 
{
    MainWindow window;
    window.StackPush(true);
    app_event_loop();
}
~~~~~~~~~~~~~~~

### Build and Run

With all that code in the `helloworld.cpp` file we just need to build the project and run it in the emulator:

```
> pebble build
> pebble install --emulator basalt
```

If you have problems with the build, check the [Known Issues](issues.html).

### Congradulations -- You've done it.
