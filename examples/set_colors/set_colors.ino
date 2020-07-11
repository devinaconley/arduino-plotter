/*
  ===========================================================================================
  Example to demonstrate configuring specific colors for each variable
  -------------------------------------------------------------------------------------------
  Plotter
  v2.4.1
  https://github.com/deviaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

double x;
double y;

Plotter p;

void setup()
{
    // Start plotter
    p.begin();

    // Add plots
    p.addTimeGraph( "Some title of a graph", 500, "label for x", x, "label for y", y );
    p.addXYGraph( "Title of X vs Y graph", 1000, "x axis", x, "y axis", y );

    // Set variable colors of graph with index 0 to pink and orange
    p.setColor( 0, "pink", "orange" );
    // Set color of x vs y graph at index 1 to cyan
    p.setColor( 1, "cyan" );
}

void loop()
{
    x = 0.0009 * sin( 2.0 * PI * ( millis() / 5000.0 ) );
    y = 90000 * cos( 2.0 * PI * ( millis() / 5000.0 ) );

    p.plot(); // usually called within loop()
}
