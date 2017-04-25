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
  v2.3.0
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#ifndef PLOTTER_H
#define PLOTTER_H

#include "Arduino.h"

class Plotter
{
public:
    // The constructor for Plotter requires no arguments
    Plotter();

    // Initialize Plotter
    void Begin();
    
    /* 
       Add a 1-variable graph vs. time
     
       Args:
       - title: String with title of graph
       - pointsDisplayed: number of points to be shown at a given time. Used to control time-scaling
       - labelA: String with label of the plotted variable
       - refA: reference to global variable that will be updated throughout program
     
       Similar methods for multi-variable graphing vs. time are declared below and follow the same format
    */
    template <typename A>
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA )
    {
	VariableWrapper * wrappers = new VariableWrapper[1];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green" );
	AddGraphHelper( title, wrappers, 1, false, pointsDisplayed );
    }
  
    /*
      Add an X vs. Y graph 
    
      Args:
      - title: String with title of graph
      - pointsDisplayed: number of points to be shown at a given time. Determines duration of data persistance
      - labelX: String with label of variable to be plotted along X-axis
      - refX: reference to global X-variable that will be updated throughout program
      - labelY: String with label of variable to be plotted along Y-axis
      - refY: reference to global Y-variable that will be updated throughout program
    */
    template <typename X, typename Y>
	void AddXYGraph( String title, int pointsDisplayed,
			 String labelX, X & refX, String labelY, Y & refY )
    {
	VariableWrapper * wrappers = new VariableWrapper[2];
	wrappers[0] = VariableWrapper( labelX, static_cast<void *>( &refX ), &Dereference<X>, "green" );
	wrappers[1] = VariableWrapper( labelY, static_cast<void *>( &refY ), &Dereference<Y>, "green" );
	AddGraphHelper( title, wrappers, 2, true, pointsDisplayed );
    }
  
    /* 
       Plot data
     
       Function to be called in order to send current values of all global variables to listener application. This 
       function will update all plots that have been added.

       It is recommended to call plot() at the end of your loop function. 
    */
    void Plot();
  
    /*
      Remove Graph

      Args:
      - index: position of graph to remove (ie. passing 0 would remove the first graph added)
      Returns:
      - true, if successful
    */
    bool Remove( int index );

    /*
      Set Variable Colors

      Args:
      - index: position of graph to set colors for
      - colorA: new color to set
      Returns:
      - true, if successful
    */
    bool SetColor( int index, String colorA );
  
    // Add a 2-variable graph vs. time
    template <typename A, typename B>
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA, String labelB, B & refB )
    {
	VariableWrapper * wrappers = new VariableWrapper[2];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green" );
	wrappers[1] = VariableWrapper( labelB, static_cast<void *>( &refB ), &Dereference<B>, "orange" );
	AddGraphHelper( title, wrappers, 2, false, pointsDisplayed );
    }

    // Add a 3-variable graph vs. time
    template <typename A, typename B, typename C>
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA, String labelB, B & refB, String labelC, C & refC )
    {
	VariableWrapper * wrappers = new VariableWrapper[3];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green" );
	wrappers[1] = VariableWrapper( labelB, static_cast<void *>( &refB ), &Dereference<B>, "orange" );
	wrappers[2] = VariableWrapper( labelC, static_cast<void *>( &refC ), &Dereference<C>, "cyan" );
	AddGraphHelper( title, wrappers, 3, false, pointsDisplayed );
    }
  
    // Add a 4-variable graph vs. time
    template <typename A, typename B, typename C, typename D>
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
			   String labelD, D & refD )
    {
	VariableWrapper * wrappers = new VariableWrapper[4];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green");
	wrappers[1] = VariableWrapper( labelB, static_cast<void *>( &refB ), &Dereference<B>, "orange" );
	wrappers[2] = VariableWrapper( labelC, static_cast<void *>( &refC ), &Dereference<C>, "cyan" );
	wrappers[3] = VariableWrapper( labelD, static_cast<void *>( &refD ), &Dereference<D>, "yellow" );
	AddGraphHelper( title, wrappers, 4, false, pointsDisplayed );
    }

    // Add a 5-variable graph vs. time
    template <typename A, typename B, typename C, typename D, typename E>
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
			   String labelD, D & refD, String labelE, E & refE )
    {
	VariableWrapper * wrappers = new VariableWrapper[5];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green" );
	wrappers[1] = VariableWrapper( labelB, static_cast<void *>( &refB ), &Dereference<B>, "orange" );
	wrappers[2] = VariableWrapper( labelC, static_cast<void *>( &refC ), &Dereference<C>, "cyan" );
	wrappers[3] = VariableWrapper( labelD, static_cast<void *>( &refD ), &Dereference<D>, "yellow" );
	wrappers[4] = VariableWrapper( labelE, static_cast<void *>( &refE ), &Dereference<E>, "pink" );
	AddGraphHelper( title, wrappers, 5, false, pointsDisplayed );
    }

    // Add a 6-variable graph vs. time
    template <typename A, typename B, typename C, typename D, typename E, typename F>    
	void AddTimeGraph( String title, int pointsDisplayed,
			   String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
			   String labelD, D & refD, String labelE, E & refE, String labelF, F & refF )
    {
	VariableWrapper * wrappers = new VariableWrapper[6];
	wrappers[0] = VariableWrapper( labelA, static_cast<void *>( &refA ), &Dereference<A>, "green" );
	wrappers[1] = VariableWrapper( labelB, static_cast<void *>( &refB ), &Dereference<B>, "orange" );
	wrappers[2] = VariableWrapper( labelC, static_cast<void *>( &refC ), &Dereference<C>, "cyan" );
	wrappers[3] = VariableWrapper( labelD, static_cast<void *>( &refD ), &Dereference<D>, "yellow" );
	wrappers[4] = VariableWrapper( labelE, static_cast<void *>( &refE ), &Dereference<E>, "pink" );
	wrappers[5] = VariableWrapper( labelF, static_cast<void *>( &refF ), &Dereference<F>, "blue" );
	AddGraphHelper( title, wrappers, 6, false, pointsDisplayed );
    }

    // Set Colors for multivariable graphs
    bool SetColor( int index, String colorA, String colorB );    
    bool SetColor( int index, String colorA, String colorB, String colorC );
    bool SetColor( int index, String colorA, String colroB, String colorC,
		   String colorD );
    bool SetColor( int index, String colorA, String colroB, String colorC,
		   String colorD, String colorE );
    bool SetColor( int index, String colorA, String colroB, String colorC,
		   String colorD, String colorE, String colorF );
    
    // Destructor for Plotter class
    ~Plotter();

public:
    
    // Nested VariableWrapper class
    class VariableWrapper
    {
    public:
        VariableWrapper();
        VariableWrapper( String label, void * ref, double ( * deref )( void * ), String color );
	
	String GetLabel();
	double GetValue();
	String GetColor();
	void SetColor( String col );

    private:
	// Data
	String label;
	String color;
	void * ref;
	double ( * deref )( void * );
	
    }; //-- VariableWrapper

    
public:
    // Nested Graph node class
    class Graph
    {
    public:
	Graph(String title, VariableWrapper * wrappers, int size, bool xvy, int pointsDisplayed);
	~Graph();
	void Plot( bool config );
	bool SetColor( int sz, String * colors );
	
	// Data
	Graph * next;
	
    private:
	bool xvy;
	int size;
	int pointsDisplayed;
	String title;
	VariableWrapper * wrappers;
    
    }; //-- Graph
    
private:
    // Helpers
    void AddGraphHelper( String title, VariableWrapper * wrappers, int sz, bool xvy, int pointsDisplayed );
    bool SetColorHelper( int index, int sz, String * colors );
    
    template <typename T>
	static double Dereference( void * ref )
    {
	return static_cast<double>( (* static_cast<T *>( ref ) ) );
    }
  
  
    // Data
    int numGraphs;
    unsigned long lastUpdated;
    int counter;
    Graph * head;
    Graph * tail;
  
}; //-- Plotter

// Constants
static const int CONFIG_INTERVAL = 50;

// Transmission Keys
static const String OUTER_KEY = "#";
static const String TIME_KEY = "t";
static const String NUM_GRAPH_KEY = "ng";
static const String LAST_UPDATED_KEY = "lu";
static const String GRAPHS_KEY = "g";
static const String TITLE_KEY = "t";
static const String XVY_KEY = "xvy";
static const String POINTS_DISPLAYED_KEY = "pd";
static const String SIZE_KEY = "sz";
static const String LABELS_KEY = "l";
static const String COLORS_KEY = "c";
static const String DATA_KEY = "d";

#endif
