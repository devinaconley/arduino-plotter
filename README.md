ArduinoPlotter
===============
An Arduino library for easy graphing on host computer via serial communication

_by: Devin Conley_

Features:
----------
- Multi-variable plots against time
- 2-variable "x" vs "y" plots
- Display multiple graphs within single window 
- Simply pass a reference to your variables when the graph is added, no need to update each value directly
- Control number of data points displayed on each graph
- Auto-scaling to fit all data on graph
- Stand-alone listener application, written with Processing, is provided

![Plotter Preview Image](https://www.dropbox.com/s/0471kf89skyo72x/plotter_preview.png?raw=1)

Quickstart:
------------

#### Install Plotter library 
Search for "Plotter" in the Arduino Library Manager.

___or___

Install manually with the [ZIP file of Plotter](https://github.com/devinconley/ArduinoPlotter-for-Library-Manager/archive/master.zip).

#### Setup Listener
Download one of the following stand-alone listener options. Keep the folder intact so the application can access the library and source folders. 
- [Windows 32-bit](https://www.dropbox.com/s/88wa2nkfzh5j3uz/ArduinoPlotter_listener_windows32.zip?dl=1)
- [Windows 64-bit](https://www.dropbox.com/s/ahy2ppul6v4lybi/ArduinoPlotter_listener_windows64.zip?dl=1)
- [Linux 32-bit](https://www.dropbox.com/s/ilt9n3hkiw74vrf/ArduinoPlotter_listener_linux32.zip?dl=1)
- [Linux 64-bit](https://www.dropbox.com/s/6irh0fn4c97aqz0/ArduinoPlotter_listener_linux64.zip?dl=1)

___or___

Download the source script from this repository (ArduinoPlotter_processingListener.pde) and run that with the Processing IDE.

#### Usage in Arduino code
Include library
```arduino
#include "Plotter.h"
```

Declare global variables. Any variable that you want plotted needs to be a double with global scope. You will also likely want the Plotter object to be accessible globally.
```arduino
double x;
double y;

Plotter p;
```

Create the Plotter object and add graphs as desired. When adding graphs, the first argument is a String with the title of the graph and the second argument is an int with the number of points displayed at any given time. These two arguments are followed by atleast one String and double pair. Include more arguments (in pairs of a String and double) to plot multiple variables on a single time graph. If adding an XYGraph, exactly two of these pairs are required. 
```arduino
void setup() {
  p = Plotter();
  
  p.addTimeGraph("Some title of single variable graph", 500, "label for x", x);
  p.addTimeGraph("Some title of two-variable graph", 500, "label for x", x, "label for y", y);
  p.addXYGraph("Title of x versus y graph", 500, "x axis", x, "y axis", y);
}
```

Update variables as desired and call plot when you want all values to be plotted on their respective graphs. This example simply assigns arbitrary sine and cosine data.
```arduino
void loop() {
  x = 10*sin(2.0*PI*(millis()/5000.0));
  y = 10*cos(2.0*PI*(millis()/5000.0));

  p.plot(); // usually called within loop()
}
```

#### Using the Listener
Once the Arduino is running, start the listener application that you setup above.

![Start Listener Image](https://www.dropbox.com/s/9kyzory64369mjh/start_listener.png?raw=1)

The application will auto-configure itself and your data should be plotted appropriately.

![Quick Start Results Image](https://www.dropbox.com/s/41w9g2htkeil6dj/quickstart_plotted.png?raw=1)