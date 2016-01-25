/*
  ===========================================================================================
  Example to demonstrate dynamic addition and removal of graphs
  -------------------------------------------------------------------------------------------
  Plotter
  v1.0.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ===========================================================================================
  */

#include "Plotter.h"

// Plotted variables must be declared as globals 
double x;
double y;
double z;

// Also declare plotter as global
Plotter p;

// Booleans so each modification only happens once
boolean add_first = true;
boolean add_second = true;
boolean remove_graph = true;

void setup() {
  // Plotter contructor
  p = Plotter();
  
  // Add a graph during setup
  p.addTimeGraph("x and y against time", 2000, "x label", &x, "y label", &y);
}

void loop() {

  // After 3 seconds add a 1-variable graph
  if (millis() > 3000 && add_first) {
    p.addTimeGraph("First dynamic addition", 2000, "z label", &z);
    add_first = false;
  }

  // After 5 seconds, add a 3-variable graph
  if (millis() > 5000 && add_second) {
    p.addTimeGraph("Second dynamic addition", 2000, "x label", &x, "y label", &y, "z label", &z);
    add_second = false;
  }

  // After 10 seconds, remove the 1-variable graph
  if (millis() > 10000 && remove_graph) {
    p.remove(1); // (an index of 1 will remove the second graph added)
    remove_graph = false;
  }
  
  // Update x, y and z variables with arbitrary sine/cosine data
  x = 3*sin(2.0*PI*(millis()/2000.0));
  y = 2*cos(2.0*PI*(millis()/2000.0));
  z = 1*sin(2.0*PI*(millis()/2000.0));
 
  // Plot 
  p.plot();
}