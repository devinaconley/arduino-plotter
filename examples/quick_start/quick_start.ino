/*
  ===========================================================================================
  Example used in Quick-Start
  -------------------------------------------------------------------------------------------
  Plotter
  v2.4.0
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

double x;

Plotter p;

void setup()
{
    p.begin();

    p.addTimeGraph( "Some title of a graph", 1500, "label for x", x );
}

void loop()
{
    x = 10 * sin( 2.0 * PI * ( millis() / 5000.0 ) );

    p.plot(); // usually called within loop()
}
