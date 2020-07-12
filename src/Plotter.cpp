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

#include "Plotter.h"
#include "Arduino.h"

// Plotter

Plotter::Plotter()
{
    head = nullptr;
    tail = nullptr;
    numGraphs = 0;
    counter = 0;
    lastUpdated = millis();
}

void Plotter::begin()
{
    Serial.begin( 115200 );
    lastUpdated = millis();
}

Plotter::~Plotter()
{
    Graph* temp = head;
    Graph* tempNext;
    while ( temp->next )
    {
        tempNext = temp->next;
        delete temp;
        temp = tempNext;
    }
    delete temp;
}

void Plotter::addGraph( Graph* graph )
{
    if ( head )
    {
        tail->next = graph;
        tail = graph;
    }
    else
    {
        head = graph;
        tail = graph;
    }

    numGraphs++;
    lastUpdated = millis();
}

bool Plotter::remove( int index )
{
    if ( numGraphs == 0 || index < 0 || numGraphs <= index )
    {
        return false;
    }
    else
    {
        Graph* temp = head;
        if ( index == 0 )
        {
            head = head->next;
            delete temp;
        }
        else
        {
            Graph* last = temp;
            for ( int i = 0; i < index; i++ )
            {
                last = temp;
                temp = temp->next;
            }
            last->next = temp->next;
            numGraphs--;
            delete temp;
        }
        lastUpdated = millis();
        return true;
    }
}

void Plotter::plot()
{
    bool config = counter == 0;

    Serial.print( "{\"" ), Serial.print( TIME_KEY );
    Serial.print( "\":" ), Serial.print( millis() );

    if ( config )
    {
        Serial.print( ",\"" ), Serial.print( NUM_GRAPH_KEY );
        Serial.print( "\":" ), Serial.print( numGraphs );
        Serial.print( ",\"" ), Serial.print( LAST_UPDATED_KEY );
        Serial.print( "\":" ), Serial.print( lastUpdated );
    }

    Serial.print( ",\"" ), Serial.print( GRAPHS_KEY ), Serial.print( "\":[" );

    Graph* temp = head;
    while ( temp )
    {
        temp->plot( config );
        temp = temp->next;
        if ( temp )
        {
            Serial.print( "," );
        }
    }
    Serial.print( "]}" ), Serial.println( OUTER_KEY );

    counter++;
    if ( counter >= CONFIG_INTERVAL )
    {
        counter = 0;
    }
}

