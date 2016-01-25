ArduinoPlotter
===============
an Arduino library for easy plotting using Processing via serial communication

_by: Devin Conley_

Types of plots available:
------------------------
* Multivariable plots against time
* 2-variable "x" vs "y" plots

Features:
----------
* Display multiple graphs within single window 
* Simply pass a reference to your variables when the graph is added, no need to update the values manually
* Choose desired number of data points to be shown along time scale for each graph
* Auto-scaling to fit all data on graph

Processing listener options:
-----------------------------
* Stand-alone Windows application (32-bit, 64-bit)
* Stand-alone Linux application (32-bit, 64-bit, armv6hf)
* Source Processing script (ArduinoPlotter_processingListener.pde)

Quickstart:
------------

#### Install Plotter library 
Search for "Plotter" in the Arduino Library Manager 
_or_
Install directly with the .zip file.

#### Setup Listener
Download this repository in order to use one of the stand-alone listener options. These stand-alone applications are dependent on your system having the Java Runtime Engine (JRE) installed.
_or_
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
Create the Plotter object and add graphs as desired. When adding graphs, the first argument is a String with the title of the graph and the second argument is an int with the number of points displayed at any given time. These two arguments are followed by atleast one String and double reference. Include more arguments (in pairs of a String and double reference) to plot multiple variables on a single time graph. If adding an XYGraph, exactly two of these pairs are required. Although graphs will usually be added during setup(), they can be added and removed at any time (see example on dynamic modification).
```arduino
void setup() {
p = Plotter();

p.addTimeGraph("Some title of single variable graph", 500, "label for x", &x);
p.addTimeGraph("Some title of two-variable graph", 500, "label for x", &x, "label for y", &y);
p.addXYGraph("Title of x versus y graph", 500, "x axis", &x, "y axis", &y);
}
```
Update variables as desired and call plot when you want all values to be plotted on their respective graphs. This example simply assigns arbitrary sine and cosine data
```arduino
void loop() {
x = 10*sin(2.0*PI*(millis()/5000.0));
y = 10*cos(2.0*PI*(millis()/5000.0));

p.plot();
}
```