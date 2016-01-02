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
  test.addTimeGraph("Title of graph", "x label", &x);
  x = 10;
  test.plot();
}

void loop() {

  if (millis() > 5000 && oneTime){
    test.addTimeGraph("Graph 2", "y label", &y, "z label", &z);
    oneTime = false;
  }
  if (millis() > 10100 && anotherOne) {
    test.addTimeGraph("Anothah one", "x", &x, "y", &y, "z", &z);
    anotherOne = false;
  }
  x = 10*sin(2.0*PI*(millis()/1000.0));
  y = 10*cos(2.0*PI*(millis()/1000.0));
  z = 5*sin(2.0*PI*(millis()/1000.0));
  test.plot();
}