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
  
  https://github.com/devinconley/Arduino-Plotter

  -------------------------------------------------------------------------------------------
  Plotter
  v2.0.0
  https://github.com/devinconley/Arduino-Plotter
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
       - refA: reference to global variable that will be updated throughout program
     
       Similar methods for multi-variable graphing vs. time are declared below and follow the same format
    */
    template <typename A>
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA )
    {
	VarWrapper * wrappers = new VarWrapper[1];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	addGraphHelper( title, wrappers, 1, false, points_displayed );
    }
  
    /*
      Add an X vs. Y graph 
    
      Args:
      - title: String with title of graph
      - points_displayed: number of points to be shown at a given time. Determines duration of data persistance
      - labelX: String with label of variable to be plotted along X-axis
      - refX: reference to global X-variable that will be updated throughout program
      - labelY: String with label of variable to be plotted along Y-axis
      - refY: reference to global Y-variable that will be updated throughout program
    */
    template <typename X, typename Y>
    void addXYGraph( String title, int points_displayed,
		     String labelX, X & refX, String labelY, Y & refY )
    {
	VarWrapper * wrappers = new VarWrapper[2];
	wrappers[0] = VarWrapper( labelX, static_cast<void *>( &refX ), &dereference<X> );
	wrappers[1] = VarWrapper( labelY, static_cast<void *>( &refY ), &dereference<Y> );
	addGraphHelper( title, wrappers, 2, true, points_displayed );
    }
  
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
    bool remove( int index );
  
    // Add a 2-variable graph vs. time
    template <typename A, typename B>
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA, String labelB, B & refB )
    {
	VarWrapper * wrappers = new VarWrapper[2];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	wrappers[1] = VarWrapper( labelB, static_cast<void *>( &refB ), &dereference<B> );
	addGraphHelper( title, wrappers, 2, false, points_displayed );
    }

    // Add a 3-variable graph vs. time
    template <typename A, typename B, typename C>
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA, String labelB, B & refB, String labelC, C & refC )
    {
	VarWrapper * wrappers = new VarWrapper[3];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	wrappers[1] = VarWrapper( labelB, static_cast<void *>( &refB ), &dereference<B> );
	wrappers[2] = VarWrapper( labelC, static_cast<void *>( &refC ), &dereference<C> );
	addGraphHelper( title, wrappers, 3, false, points_displayed );
    }
  
    // Add a 4-variable graph vs. time
    template <typename A, typename B, typename C, typename D>
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
		       String labelD, D & refD )
    {
	VarWrapper * wrappers = new VarWrapper[4];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	wrappers[1] = VarWrapper( labelB, static_cast<void *>( &refB ), &dereference<B> );
	wrappers[2] = VarWrapper( labelC, static_cast<void *>( &refC ), &dereference<C> );
	wrappers[3] = VarWrapper( labelD, static_cast<void *>( &refD ), &dereference<D> );
	addGraphHelper( title, wrappers, 4, false, points_displayed );
    }

    // Add a 5-variable graph vs. time
    template <typename A, typename B, typename C, typename D, typename E>
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
		       String labelD, D & refD, String labelE, E & refE )
    {
	VarWrapper * wrappers = new VarWrapper[5];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	wrappers[1] = VarWrapper( labelB, static_cast<void *>( &refB ), &dereference<B> );
	wrappers[2] = VarWrapper( labelC, static_cast<void *>( &refC ), &dereference<C> );
	wrappers[3] = VarWrapper( labelD, static_cast<void *>( &refD ), &dereference<D> );
	wrappers[4] = VarWrapper( labelE, static_cast<void *>( &refE ), &dereference<E> );
	addGraphHelper( title, wrappers, 5, false, points_displayed );
    }

    // Add a 6-variable graph vs. time
    template <typename A, typename B, typename C, typename D, typename E, typename F>    
    void addTimeGraph( String title, int points_displayed,
		       String labelA, A & refA, String labelB, B & refB, String labelC, C & refC,
		       String labelD, D & refD, String labelE, E & refE, String labelF, F & refF )
    {
	VarWrapper * wrappers = new VarWrapper[6];
	wrappers[0] = VarWrapper( labelA, static_cast<void *>( &refA ), &dereference<A> );
	wrappers[1] = VarWrapper( labelB, static_cast<void *>( &refB ), &dereference<B> );
	wrappers[2] = VarWrapper( labelC, static_cast<void *>( &refC ), &dereference<C> );
	wrappers[3] = VarWrapper( labelD, static_cast<void *>( &refD ), &dereference<D> );
	wrappers[4] = VarWrapper( labelE, static_cast<void *>( &refE ), &dereference<E> );
	wrappers[4] = VarWrapper( labelF, static_cast<void *>( &refF ), &dereference<F> );
	addGraphHelper( title, wrappers, 6, false, points_displayed );
    }
		    
    // Destructor for Plotter class
    ~Plotter();
  
public:

    // Nested VariableWrapper class
    class VarWrapper
    {
    public:
        VarWrapper();
        VarWrapper( String label, void * ref, double ( * deref )( void * ) );

	String GetLabel();
	double GetValue();

    private:
	// Data
	String label;
	void * ref;
	double ( * deref )( void * );
      
    }; //-- VarWrapper

public:
    // Nested Graph node class
    class Graph
    {
    public:
	Graph(String title, VarWrapper * wrappers, int size, bool xvy, int points_displayed);
	~Graph();
	void plot();
      
	// Data
	Graph * next;
	int size;

    private:
	bool xvy;
	int points_displayed;
	String title;
	VarWrapper * wrappers;
    
    }; //-- Graph
  
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
  
}; //-- Plotter

static const String OUTER_KEY = "#";
static const String INNER_KEY = "@";


#endif
