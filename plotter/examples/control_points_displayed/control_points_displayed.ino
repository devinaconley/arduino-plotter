/*
  ===========================================================================================
  Example to demonstrate effect of "Points Displayed" in Time Graph and in XY Graph
  -------------------------------------------------------------------------------------------
  Plotter
  v2.3.0
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

// Plotted variables must be declared as globals 
double x;
double y;

// Also declare plotter as global
Plotter p;

void setup()
{
    // Start plotter
    p.Begin();
  
    // Add X-Y graphs 
    p.AddXYGraph( "X-Y graph w/ 500 points", 500, "x axis", x, "y axis", y );
    p.AddXYGraph( "X-Y graph w/ 200 points", 200, "x axis", x, "y axis", y );

    // Add time graphs. Notice the effect of points displayed on the time scale
    p.AddTimeGraph( "Time graph w/ 500 points", 500, "x label", x );
    p.AddTimeGraph( "Time graph w/ 200 points", 200, "x label", x );

}

void loop()
{
    // Update variables with arbitrary sine/cosine data
    x = 10*sin( 2.0*PI*( millis() / 5000.0 ) );
    y = 10*cos( 2.0*PI*( millis() / 5000.0 ) );

    // Plot
    p.Plot();
}
