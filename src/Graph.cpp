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

#include "Graph.h"
#include "Arduino.h"

// Graph

Graph::Graph( const char* title, int maxPoints )
    : next( nullptr ),
      title( title ),
      maxPoints( maxPoints ),
      size( 0 ),
      head( nullptr ),
      tail( nullptr )
{
}

Graph::~Graph()
{
    Variable* temp = head;
    Variable* tempNext;
    while ( temp->next )
    {
        tempNext = temp->next;
        delete temp;
        temp = tempNext;
    }
    delete temp;
}

void Graph::addVariable( Variable* var )
{
    if ( head )
    {
        tail->next = var;
        tail = var;
    }
    else
    {
        head = var;
        tail = var;
    }
    size++;
}

void Graph::plot( bool config )
{
    Serial.print( "{" );

    if ( config )
    {
        Serial.print( "\"" ), Serial.print( TITLE_KEY ), Serial.print( "\":" );
        Serial.print( "\"" ), Serial.print( title ), Serial.print( "\"" );
        Serial.print( ",\"" ), Serial.print( XVY_KEY );
        Serial.print( "\":" ), Serial.print( xvy() );
        Serial.print( ",\"" ), Serial.print( POINTS_DISPLAYED_KEY );
        Serial.print( "\":" ), Serial.print( maxPoints );
        Serial.print( ",\"" ), Serial.print( SIZE_KEY );
        Serial.print( "\":" ), Serial.print( size );
        Serial.print( ",\"" ), Serial.print( LABELS_KEY ), Serial.print( "\":[" );
        Variable* temp = head;
        while ( temp )
        {
            Serial.print( "\"" ), Serial.print( temp->getLabel() ), Serial.print( "\"" );
            temp = temp->next;
            if ( temp )
            {
                Serial.print( "," );
            }
        }
        Serial.print( "],\"" ), Serial.print( COLORS_KEY ), Serial.print( "\":[" );
        temp = head;
        while ( temp )
        {
            Serial.print( "\"" ), Serial.print( temp->getColor() ), Serial.print( "\"" );
            temp = temp->next;
            if ( temp )
            {
                Serial.print( "," );
            }
        }
        Serial.print( "]," );
    }

    Serial.print( "\"" ), Serial.print( DATA_KEY ), Serial.print( "\":[" );
    Variable* temp = head;
    while ( temp )
    {
        Serial.print( temp->getValue() );
        temp = temp->next;
        if ( temp )
        {
            Serial.print( "," );
        }
    }
    Serial.print( "]}" );
}

// Line graph

LineGraph::LineGraph( const char* title = "", int range = 5000, int maxPoints = 1000 )
    : Graph( title, maxPoints ), range( range )
{
}

// Scatter graph

ScatterGraph::ScatterGraph( const char* title = "", int maxPoints = 1000 )
    : Graph( title, maxPoints )
{
}

// Variable

Graph::Variable::Variable() : ref( nullptr ), deref( nullptr ), next( nullptr ) {}

Graph::Variable::Variable(
    const char* label,
    void* ref,
    double ( *deref )( void* ),
    const char* color )
    : label( label ), color( color ), ref( ref ), deref( deref ), next( nullptr )
{
}

const char* Graph::Variable::getLabel()
{
    return label;
}

double Graph::Variable::getValue()
{
    return deref( ref );
}

const char* Graph::Variable::getColor()
{
    return color;
}
