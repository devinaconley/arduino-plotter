/*
  ===========================================================================================
  Example to demonstrate effect of "Points Displayed" in Time Graph and in XY Graph
  -------------------------------------------------------------------------------------------
  Plotter
  v1.0.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ===========================================================================================
  */

#include "Plotter.h"

// Plotted variables must be declared as globals 
double x;
double y;

// Also declare plotter as global
Plotter p;

void setup() {
  // Plotter constructor
  p = Plotter();
  
  // Add X-Y graphs 
  p.addXYGraph("X-Y graph w/ 500 points", 500, "x axis", &x, "y axis", &y);
  p.addXYGraph("X-Y graph w/ 200 points", 200, "x axis", &x, "y axis", &y);

  // Add time graphs. Notice the effect of points displayed on the time scale
  p.addTimeGraph("Time graph w/ 100 points", 1000, "x label", &x);
  p.addTimeGraph("Time graph w/ 500 points", 500, "x label", &x);

}

void loop() {

  // Update variables with arbitrary sine/cosine data
  x = 10*sin(2.0*PI*(millis()/5000.0));
  y = 10*cos(2.0*PI*(millis()/5000.0));

  // Plot
  p.plot();
}
