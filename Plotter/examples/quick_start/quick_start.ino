/*
  ===========================================================================================
  Example used in Quick-Start
  -------------------------------------------------------------------------------------------
  Plotter
  v2.0.0
  https://github.com/devinconley/Arduino-Plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

double x;

Plotter p;

void setup() {
  p = Plotter();

  p.addTimeGraph( "Some title of a graph", 500, "label for x", x );
}

void loop() {
  x = 10*sin( 2.0*PI*( millis() / 5000.0 ) );

  p.plot(); // usually called within loop()
}