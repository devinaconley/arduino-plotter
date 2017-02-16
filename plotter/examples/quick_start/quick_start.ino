/*
  ===========================================================================================
  Example used in Quick-Start
  -------------------------------------------------------------------------------------------
  Plotter
  v2.2.0
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

double x;

Plotter p;

void setup()
{
    p = Plotter();

    p.AddTimeGraph( "Some title of a graph", 500, "label for x", x );
}

void loop() {
    x = 10*sin( 2.0*PI*( millis() / 5000.0 ) );

    p.Plot(); // usually called within loop()
}