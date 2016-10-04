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
  template <typename T>
      void addTimeGraph(String title, int points_displayed, String labelA, T & refA)
  {
      VarWrapper * temp = new VarWrapper[1];
      temp[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<T> );
      addGraphHelper( title, temp, 1, false, points_displayed );
  }

  
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
  
/*
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
*/
		    
// Destructor for Plotter class
  ~Plotter();
  
 public:

  // Nested VariableWrapper class
  class VarWrapper
  {
  public:
      VarWrapper() : ref( NULL ), deref( NULL ) {}
      VarWrapper( String label, void * ref, double ( * deref )( void * ) ) :
          label( label ), ref( ref ), deref( deref )
      {}

      String GetLabel()
      {
	  return label;
      }
      
      double GetValue()
      {
	  return deref( ref );
      }

  private:
      // Data
      double ( * deref )( void * );
      void * ref;
      String label;
      
  }; // --VarWrapper

public:
  // Nested Graph node class
  class Graph
  {
  public:
    Graph * next;
    int size;
    Graph(String title, VarWrapper * wrappers, int size, bool xvy, 
	      int points_displayed);
    void plot();
    
  private:
    bool xvy;
    int points_displayed;
    String title;
    VarWrapper * wrappers;
    
  }; // --Graph
  
 private:
  // Helpers
  void addGraphHelper(String title, VarWrapper * wrappers, int sz, bool xvy, int points_displayed);
  
  template <typename T>
  static double dereference( void * ref )
  {
      return (double) (* static_cast<T *>( ref ) );
  }
  
  
  // Data
  int num_graphs;
  int total_size;
  int max_points_displayed;
  unsigned long last_updated;
  Graph * head;
  Graph * tail;
  
}; // --Plotter

static const String OUTER_KEY = "#";
static const String INNER_KEY = "@";


#endif
