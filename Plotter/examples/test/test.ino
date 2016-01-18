// Test script that runs dev library
#include "Plotter.h"

// Globals needed for plotting 
double x;
double y;
double z;
double a;
Plotter test;

boolean oneTime = true;
boolean anotherOne = true;

void setup() {
  Serial.begin(9600);
  test = Plotter();

  test.addXYGraph("X-Z graph", 3000, "x", &x, "z", &z);
  test.addTimeGraph("4 variable plot :) ", 1000, "label for the x variable", &x, "label for the y var", &y, "label for zeta", &z, "label for alpha", &a);
  x = 10;
  a = 0;
  test.plot();
}

void loop() {

  if (millis() > 3000 && anotherOne) {
    test.addTimeGraph("Anothah one", 2000, "x", &x, "y", &y);
    anotherOne = false;
  }
  
  if (millis() % 2 == 0) {
    a += 1;
  }else{
    a -= 1;
  }
  x = 10*sin(2.0*PI*(millis()/10000.0));
  y = 10*cos(2.0*PI*(millis()/10000.0));
  z = 5*sin(2.0*PI*(millis()/10000.0));
  test.plot();
}