/*
  ===========================================================================================
  Plotter is an Arduino library that allows easy multi-variable and multi-graph plotting. The
  library supports plots against time as well as 2-variable "X vs Y" graphing. 
  -------------------------------------------------------------------------------------------
  The library transfers information via the serial port to a listener program written with the
  software provided by Processing. No modification is needed to this program; graph placement,
  axis-scaling, etc. are handled automatically. 
  Multiple options for this listener are available including stand-alone applications as well 
  as the source Processing script.

  The library, these listeners, a quick-start guide, and usage examples are available at:
  
  https://github.com/devinconley/ArduinoPlotter

  -------------------------------------------------------------------------------------------
  Plotter
  v1.1.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ===========================================================================================
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include "Arduino.h"

class Plotter {
 public:
  // The constructor for Plotter requires no arguments
  Plotter();
  
  /* 
     Add a 1-variable graph vs. time
     
     Args:
     - title: String with title of graph
     - points_displayed: number of points to be shown at a given time. Used to control time-scaling
     - labelA: String with label of the plotted variable
     - refA: reference to global double variable that will be updated throughout program
     
     Similar methods for multi-variable graphing vs. time are declared below and follow the same format
   */
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA);

  /*
    Add an X vs. Y graph 
    
    Args:
    - title: String with title of graph
    - points_displayed: number of points to be shown at a given time. Determines duration of data persistance
    - labelX: String with label of variable to be plotted along X-axis
    - refX: reference to global double X-variable that will be updated throughout program
    - labelY: String with label of variable to be plotted along Y-axis
    - refY: reference to global double Y-variable that will be updated throughout program
  */
  void addXYGraph(String title, int points_displayed,
		  String labelX, double & refX, String labelY, double & refY);
  
  /* 
     Plot data
     
     Function to be called in order to send current values of all global variables to listener application. This 
     function will update all plots that have been added.

     It is recommended to call plot() at the end of your loop function. 
   */
  void plot();
  
  
  /*
    Remove Graph

    Args:
    - index: position of graph to remove (ie. passing 0 would remove the first graph added)
    Returns:
    - true, if successful
   */
  bool remove(int index);
  

  // Add a 2-variable graph vs. time
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
		    String labelB, double & refB);
  // Add a 3-variable graph vs. time
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
		    String labelB, double & refB, String labelC, double & refC);
  
  // Add a 4-variable graph vs. time
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
		    String labelB, double & refB, String labelC, double & refC,
		    String labelD, double & refD);
  // Add a 5-variable graph vs. time
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
		    String labelB, double & refB, String labelC, double & refC,
		    String labelD, double & refD, String labelE, double & refE);
  // Add a 6-variable graph vs. time
  void addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
		    String labelB, double & refB, String labelC, double & refC,
		    String labelD, double & refD, String labelE, double & refE,
		    String labelF, double & refF);
  
  // Destructor for Plotter class
  ~Plotter();
  
 private:
  // Internal helper class
  class GraphNode {
  public:
    GraphNode* next;
    int size;
    GraphNode(String title, String* labels, double** refs, int size, bool xvy, 
	      int points_displayed);
    void plot();
  private:
    bool xvy;
    int points_displayed;
    String title;
    String* labels;
    double** refs;
  };
 private:
  // Private members
  int num_graphs;
  int total_size;
  int max_points_displayed;
  unsigned long last_updated;
  GraphNode* head;
  GraphNode* tail;
  void addGraphHelper(String title, String labels[], double* refs[], int sz, bool xvy, int points_displayed);
};

static const String OUTER_KEY = "#";
static const String INNER_KEY = "@";


#endif
