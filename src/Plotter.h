/*
  ===========================================================================================
  Plotter is an Arduino library that allows easy multi-variable and multi-graph plotting. The
  library supports plots against time as well as 2-variable "X vs Y" graphing.
  -------------------------------------------------------------------------------------------
  The library stores and handles all relevant graph information and variable references,
  and transfers information via the serial port to a listener program written with the
  software provided by Processing. No modification is needed to this program; graph placement,
  axis-scaling, etc. are handled automatically.
  Multiple options for this listener are available including stand-alone applications as well
  as the source Processing script.

  The library, these listeners, a quick-start guide, documentation, and usage examples are
  available at:

  https://github.com/devinaconley/arduino-plotter

  -------------------------------------------------------------------------------------------
  Plotter
  v2.4.1
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include "Graph.h"
#include "Util.h"

class Plotter
{
  public:
    // The constructor for Plotter requires no arguments
    Plotter();

    // Initialize Plotter
    void begin();

    /*
       Add an empty line graph vs. time

       Args:
       - title: const char * with title of graph
       - maxPoints: number of points to be shown at a given time. Used to control time-scaling
    */
    LineGraph& addLineGraph( const char* title, int maxPoints = 1000 )
    {
        LineGraph* graph = new LineGraph( title, 5000, maxPoints );
        addGraph( graph );
        return *graph;
    }

    /*
       Add a 1-variable graph vs. time

       Args:
       - title: const char * with title of graph
       - maxPoints: number of points to be shown at a given time. Used to control time-scaling
       - labelA: const char * with label of the plotted variable
       - refA: reference to global variable that will be updated throughout program

       Similar methods for multi-variable graphing vs. time are declared below and follow the same
       format
    */
    template <typename A>
    void addLineGraph( const char* title, int maxPoints, const char* labelA, A& refA )
    {
        LineGraph* graph = new LineGraph( title, 5000, maxPoints );
        graph->line( labelA, refA );
        addGraph( graph );
    }

    /*
      Add an X vs. Y graph

      Args:
      - title: const char * with title of graph
      - maxPoints: max number of points to be shown at a given time. Controls data persistence
      - labelX: const char * with label of variable to be plotted along X-axis
      - refX: reference to global X-variable that will be updated throughout program
      - labelY: const char * with label of variable to be plotted along Y-axis
      - refY: reference to global Y-variable that will be updated throughout program
    */
    template <typename X, typename Y>
    void addScatterGraph(
        const char* title,
        int maxPoints,
        const char* labelX,
        X& refX,
        const char* labelY,
        Y& refY )
    {
        ScatterGraph* graph = new ScatterGraph( title, maxPoints );
        graph->scatter( labelX, refX, labelY, refY );
        addGraph( graph );
    }

    /*
       Plot data

       Function to be called in order to send current values of all global variables to listener
       application. This function will update all plots that have been added.

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
    bool remove( int index );

    // Add a 2-variable graph vs. time
    template <typename A, typename B>
    void addLineGraph(
        const char* title,
        int maxPoints,
        const char* labelA,
        A& refA,
        const char* labelB,
        B& refB )
    {
        LineGraph* graph = new LineGraph( title, 5000, maxPoints );
        graph->line( labelA, refA );
        graph->line( labelB, refB );
        addGraph( graph );
    }

    // Add a 3-variable graph vs. time
    template <typename A, typename B, typename C>
    void addLineGraph(
        const char* title,
        int maxPoints,
        const char* labelA,
        A& refA,
        const char* labelB,
        B& refB,
        const char* labelC,
        C& refC )
    {
        LineGraph* graph = new LineGraph( title, 5000, maxPoints );
        graph->line( labelA, refA );
        graph->line( labelB, refB );
        graph->line( labelC, refC );
        addGraph( graph );
    }

    // Destructor for Plotter class
    ~Plotter();

  private:
    // Helpers
    void addGraph( Graph* graph );

    // Data
    int numGraphs;
    unsigned long lastUpdated;
    int counter;
    Graph* head;
    Graph* tail;
}; //-- Plotter

#endif
