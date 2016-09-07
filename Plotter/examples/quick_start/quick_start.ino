/*
  ===========================================================================================
  Example to demonstrate multi-variable plotting against time
  -------------------------------------------------------------------------------------------
  Plotter
  v1.1.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

double x;
double y;

Plotter p;

void setup() {
  p = Plotter();

  p.addTimeGraph("Some title of single variable graph", 500, "label for x", x);
  p.addTimeGraph("Some title of two-variable graph", 500, "label for x", x, "label for y", y);
  p.addXYGraph("Title of x versus y graph", 500, "x axis", x, "y axis", y);
}

void loop() {
  x = 10*sin(2.0*PI*(millis()/5000.0));
  y = 10*cos(2.0*PI*(millis()/5000.0));

  p.plot(); // usually called within loop()
}