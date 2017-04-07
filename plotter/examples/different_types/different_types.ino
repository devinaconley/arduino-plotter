/*
  ===========================================================================================
  Example to demonstrate effect of plotting different variable types
  -------------------------------------------------------------------------------------------
  Plotter
  v2.3.0
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#include "Plotter.h"

// Plotted variables must be declared as globals 
float f;
float f2;
int i;
char ch;

// Also declare plotter as global
Plotter p;

void setup()
{
    // Start plotter
    p.Begin();
  
    // Add time graphs.
    p.AddTimeGraph( "float vs int", 500, "float", f, "int", i );
    p.AddTimeGraph( "float vs char", 500, "float", f2, "unsigned char", ch );

}

void loop()
{
    // Update different variable types with arbitrary sine/cosine data
    unsigned long time = millis();
    f = 5*sin( 2.0*PI*( time / 5000.0 ) );
    i = 5*sin( 2.0*PI*( time / 5000.0 ) );
    f2 = 300*sin( 2.0*PI*( time / 5000.0 ) );
    ch = 300*sin( 2.0*PI*( time / 5000.0 ) );

    // Plot
    p.Plot();
}
