/*
  ///////////////////////////////////////////////////////////////////////////////////////////
  Plotter is an Arduino library that allows easy multi-variable and multi-graph plotting. The
  library supports plots against time as well as 2-variable "X vs Y" graphing. 
  -------------------------------------------------------------------------------------------
  The library transfers information via the serial port to a listener program written with the
  software provided by Processing. No modification is needed to this program; graph placement,
  axis-scaling, etc. are handled automatically. 
  Multiple options for this listener are avaialable at: 
  
  https://github.com/devinconley/ArduinoPlotter

  (including stand-alone applications as well as the src Processing script) 
  -------------------------------------------------------------------------------------------
  Plotter
  v0.0.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ///////////////////////////////////////////////////////////////////////////////////////////
 */

#ifndef PLOTTER_H
#define PLOTTER_H

#include "Arduino.h"

class Plotter {
 public:
  Plotter();
  ~Plotter();
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA);
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
		    String labelB, double* refB);
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
		    String labelB, double* refB, String labelC, double* refC);
  
  void addXYGraph(String title, int points_displayed,
		  String labelX, double* refX, String labelY, double* refY);
  void plot();


 private:
  // Internal helper class
  class GraphNode {
  public:
    GraphNode* next;
    GraphNode(String title, String* labels, double** refs, int size, bool xvy, 
	      int points_displayed);
    void plot();
  private:
    bool xvy;
    int size;
    int points_displayed;
    String title;
    String* labels;
    double** refs;
  };
  // Private members
  int num_graphs;
  int total_size;
  int max_points_displayed;
  unsigned long last_updated;
  GraphNode* head;
  GraphNode* tail;
  
};

static const String OUTER_KEY = "#";
static const String INNER_KEY = "@";


#endif
