/*
  ===========================================================================================
  Graph classes
  -------------------------------------------------------------------------------------------
  Plotter
  v2.4.1
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#ifndef GRAPH_H
#define GRAPH_H

#include "Util.h"

// Abstract graph class / node
class Graph
{
  public:
    Graph( const char* title, int maxPoints );
    ~Graph();
    virtual void plot( bool config );

    // Data
    Graph* next;

  protected:
    // Nested Variable wrapper class
    class Variable
    {
      public:
        Variable();
        Variable( const char* label, void* ref, double ( *deref )( void* ), const char* color );

        const char* getLabel();
        double getValue();
        const char* getColor();

        Variable* next;

      private:
        // Data
        const char* label;
        const char* color;
        void* ref;
        double ( *deref )( void* );

    }; //-- Variable

    template <typename T> static double dereference( void* ref )
    {
        return static_cast<double>( ( *static_cast<T*>( ref ) ) );
    }

  protected:
    void addVariable( Variable* variable );
    virtual bool xvy() = 0;
    int size;
    int maxPoints;
    const char* title;
    Variable* head;
    Variable* tail;
};

// Line graph vs time
class LineGraph : public Graph
{
  public:
    LineGraph( const char* title = "", int range = 5000, int maxPoints = 1000 );

    template <typename T> void line( const char* label, T& ref, const char* color = "" )
    {
        if ( color[0] == '\0' )
        {
            color = COLORS[size % 6];
        }
        addVariable( new Variable( label, static_cast<void*>( &ref ), &dereference<T>, color ) );
    }

    virtual bool xvy()
    {
        return false;
    }

  private:
    // Data
    int range;
};

// X vs. Y graph
class ScatterGraph : public Graph
{
  public:
    ScatterGraph( const char* title = "", int maxPoints = 1000 );

    template <typename X, typename Y>
    void scatter( const char* labelX, X& refX, const char* labelY, Y& refY, const char* color = "" )
    {
        if ( color[0] == '\0' )
        {
            color = COLORS[( size / 2 ) % 6];
        }
        addVariable( new Variable( labelX, static_cast<void*>( &refX ), &dereference<X>, color ) );
        addVariable( new Variable( labelY, static_cast<void*>( &refY ), &dereference<Y>, color ) );
    }

    virtual bool xvy()
    {
        return true;
    }
};

#endif
