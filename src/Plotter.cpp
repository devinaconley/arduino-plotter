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

// Plotter

Plotter::Plotter()
{
    head = NULL;
    tail = NULL;
    numGraphs = 0;
    counter = 0;
    lastUpdated = millis();
}

void Plotter::Begin()
{
    Serial.begin( 115200 );
    lastUpdated = millis();
}

Plotter::~Plotter()
{ 
    Graph * temp = head;
    Graph * tempNext;
    while ( temp->next )
    {
	tempNext = temp->next;
	delete temp;
	temp = tempNext;
    }
    delete temp;
}

void Plotter::AddGraphHelper( const char * title, VariableWrapper * wrappers, int sz, bool xvy, int pointsDisplayed )
{ 
    Graph * temp = new Graph( title, wrappers, sz, xvy, pointsDisplayed );
    if ( head )
    {
	tail->next = temp;
	tail = temp;
    }
    else
    {
	head = temp;
	tail = temp;
    }
  
    numGraphs++;  
    lastUpdated = millis();
}

bool Plotter::Remove( int index )
{
    if ( numGraphs == 0 || index < 0 || numGraphs <= index )
    {
	return false;
    }
    else
    {
	Graph * temp = head;
	if ( index == 0 )
	{
	    head = head->next;
	    delete temp;
	}
	else
	{
	    Graph * last = temp;
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

bool Plotter::SetColor( int index, const char * colorA )
{
    const char * colors[] = { colorA };
    return SetColorHelper( index, 1, colors );
}

bool Plotter::SetColor( int index, const char * colorA, const char * colorB )
{
    const char * colors[] = { colorA, colorB };
    return SetColorHelper( index, 2, colors );
}

bool Plotter::SetColor( int index, const char * colorA, const char * colorB, const char * colorC )
{
    const char * colors[] = { colorA, colorB, colorC };
    return SetColorHelper( index, 3, colors );
}

bool Plotter::SetColor( int index, const char * colorA, const char * colorB, const char * colorC,
			const char * colorD )
{
    const char * colors[] = { colorA, colorB, colorC, colorD };
    return SetColorHelper( index, 4, colors );
}

bool Plotter::SetColor( int index, const char * colorA, const char * colorB, const char * colorC,
			const char * colorD, const char * colorE )
{
    const char * colors[] = { colorA, colorB, colorC, colorD, colorE };
    return SetColorHelper( index, 5, colors );
}

bool Plotter::SetColor( int index, const char * colorA, const char * colorB, const char * colorC,
			const char * colorD, const char * colorE, const char * colorF )
{
    const char * colors[] = { colorA, colorB, colorC, colorD, colorE, colorF };
    return SetColorHelper( index, 6, colors );
}

bool Plotter::SetColorHelper( int index, int sz, const char * * colors )
{
    if ( numGraphs == 0 || index < 0 || numGraphs <= index )
    {
	return false;
    }
    Graph * temp = head;
    for ( int i = 0; i < index; i++ )
    {
	temp = temp->next;
    }
    bool res = temp->SetColor( sz, colors );
    if ( res )
    {
	lastUpdated = millis();
    }
    return res;
}

void Plotter::Plot()
{
    bool config = counter == 0;

    Serial.print( "{\"" ); Serial.print( TIME_KEY ); Serial.print( "\":" ); Serial.print( millis() );
    
    if ( config )
    {
	Serial.print( ",\"" ); Serial.print( NUM_GRAPH_KEY ); Serial.print( "\":" ); Serial.print( numGraphs );
	Serial.print( ",\"" ); Serial.print( LAST_UPDATED_KEY ); Serial.print( "\":" ); Serial.print( lastUpdated );
    }
    
    Serial.print( ",\"" ); Serial.print( GRAPHS_KEY ); Serial.print( "\":[" );

    Graph * temp = head;
    while ( temp )
    {
	temp->Plot( config );
	temp = temp->next;
	if ( temp )
	{
	    Serial.print( "," );
	}
    }
    Serial.print( "]}" ); Serial.println( OUTER_KEY );

    counter++;
    if ( counter >= CONFIG_INTERVAL )
    {
	counter = 0;
    }
}

// Graph

Plotter::Graph::Graph( const char * title, VariableWrapper * wrappers, int size, bool xvy, int pointsDisplayed ) :
    next( NULL ),
    xvy( xvy ),
    size( size ),
    pointsDisplayed( pointsDisplayed ),
    title( title ),
    wrappers( wrappers )
{}

Plotter::Graph::~Graph()
{
    delete[] wrappers;
}

void Plotter::Graph::Plot( bool config )
{
    Serial.print( "{" );

    if ( config )
    {
	Serial.print( "\"" ); Serial.print( TITLE_KEY ); Serial.print( "\":" ); Serial.print( "\"" ); Serial.print( title ); Serial.print( "\"" );
	Serial.print( ",\"" ); Serial.print( XVY_KEY ); Serial.print( "\":" ); Serial.print( xvy );
	Serial.print( ",\"" ); Serial.print( POINTS_DISPLAYED_KEY ); Serial.print( "\":" ); Serial.print( pointsDisplayed );
	Serial.print( ",\"" ); Serial.print( SIZE_KEY ); Serial.print( "\":" ); Serial.print( size );
	Serial.print( ",\"" ); Serial.print( LABELS_KEY ); Serial.print( "\":[" );
	for ( int i = 0; i < size; i++ )
	{
	    Serial.print( "\"" ); Serial.print( wrappers[i].GetLabel() ); Serial.print( "\"" );
	    if ( i + 1 < size )
	    {
		Serial.print( "," );
	    }
	}
	Serial.print( "],\"" ); Serial.print( COLORS_KEY ); Serial.print( "\":[" );
	for ( int i = 0; i < size; i++ )
	{
	    Serial.print( "\"" ); Serial.print( wrappers[i].GetColor() ); Serial.print( "\"" );
	    if ( i + 1 < size )
	    {
		Serial.print( "," );
	    }
	}
	Serial.print( "]," );
    }
    
    Serial.print( "\"" ); Serial.print( DATA_KEY ); Serial.print( "\":[" );
    for ( int i = 0; i < size; i++ )
    {
	Serial.print( wrappers[i].GetValue(), 8 );
	if ( i + 1 < size )
	{
	    Serial.print( "," );
	}
    }
    
    Serial.print( "]}" );
}

bool Plotter::Graph::SetColor( int sz, const char * * colors )
{
    if ( sz != size && !xvy )
    {
	return false;
    }

    if ( xvy )
    {
	wrappers[0].SetColor( colors[0] );
    }
    else
    {
	for ( int i = 0; i < size; i++ )
	{
	    wrappers[i].SetColor( colors[i] );
	}
    }
    return true;
}

// VariableWrapper

Plotter::VariableWrapper::VariableWrapper() :
    ref( NULL ),
    deref( NULL )
{}

Plotter::VariableWrapper::VariableWrapper( const char * label, void * ref, double ( * deref )( void * ), const char * color ) :
    label( label ),
    color( color ),
    ref( ref ),
    deref( deref )
{}

const char * Plotter::VariableWrapper::GetLabel()
{
    return label;
}

double Plotter::VariableWrapper::GetValue()
{
    return deref( ref );
}

const char * Plotter::VariableWrapper::GetColor()
{
    return color;
}

void Plotter::VariableWrapper::SetColor( const char * col )
{
    color = col;
}
