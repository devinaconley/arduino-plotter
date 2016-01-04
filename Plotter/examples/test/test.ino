// Test script that runs dev library
#include "Plotter.h"

// Globals needed for plotting 
double x;
double y;
double z;
Plotter test;

boolean oneTime = true;
boolean anotherOne = true;

void setup() {
  Serial.begin(9600);
  test = Plotter();

  test.addXYGraph("X-Y graph", 3000, "x", &x, "y", &y);
  x = 10;
  test.plot();
}

void loop() {

  if (millis() > 10100 && anotherOne) {
    test.addTimeGraph("Anothah one", 3000, "x", &x, "y", &y, "z", &z);
    anotherOne = false;
  }

  x = 10*sin(2.0*PI*(millis()/10000.0));
  y = 10*cos(2.0*PI*(millis()/10000.0));
  z = 5*sin(2.0*PI*(millis()/10000.0));
  test.plot();
}