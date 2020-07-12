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
    LineGraph& lg = p.addLineGraph( "Some title of a graph", 500 );
    ScatterGraph& sg = p.addScatterGraph( "Title of X vs Y graph", 1000 );

    // Add line variables and specify color
    lg.line( "label for x", x, "pink" );
    lg.line( "label for y", y, "orange" );

    // Add scatter channel and specify color
    sg.scatter( "x axis", x, "y axis", y, "cyan" );
}

void loop()
{
    x = 0.0009 * sin( 2.0 * PI * ( millis() / 5000.0 ) );
    y = 90000 * cos( 2.0 * PI * ( millis() / 5000.0 ) );

    p.plot(); // usually called within loop()
}
