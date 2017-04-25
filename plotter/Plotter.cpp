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

#include "Plotter.h"

// Plotter

Plotter::Plotter()
{
    head = NULL;
    tail = NULL;
    numGraphs = 0;
    counter = 0;
    Serial.begin( 115200 ); // duplicate calls left for backward compatibility
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

void Plotter::AddGraphHelper( String title, VariableWrapper * wrappers, int sz, bool xvy, int pointsDisplayed )
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

bool Plotter::SetColor( int index, String colorA )
{
    String colors[] = { colorA };
    return SetColorHelper( index, 1, colors );
}

bool Plotter::SetColor( int index, String colorA, String colorB )
{
    String colors[] = { colorA, colorB };
    return SetColorHelper( index, 2, colors );
}

bool Plotter::SetColor( int index, String colorA, String colorB, String colorC )
{
    String colors[] = { colorA, colorB, colorC };
    return SetColorHelper( index, 3, colors );
}

bool Plotter::SetColor( int index, String colorA, String colorB, String colorC,
			String colorD )
{
    String colors[] = { colorA, colorB, colorC, colorD };
    return SetColorHelper( index, 4, colors );
}

bool Plotter::SetColor( int index, String colorA, String colorB, String colorC,
			String colorD, String colorE )
{
    String colors[] = { colorA, colorB, colorC, colorD, colorE };
    return SetColorHelper( index, 5, colors );
}

bool Plotter::SetColor( int index, String colorA, String colorB, String colorC,
			String colorD, String colorE, String colorF )
{
    String colors[] = { colorA, colorB, colorC, colorD, colorE, colorF };
    return SetColorHelper( index, 6, colors );
}

bool Plotter::SetColorHelper( int index, int sz, String * colors )
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

    Serial.print( "{\"" + TIME_KEY + "\":" ); Serial.print( millis() );
    
    if ( config )
    {
	Serial.print( ",\"" + NUM_GRAPH_KEY + "\":" ); Serial.print( numGraphs );
	Serial.print( ",\"" + LAST_UPDATED_KEY + "\":" ); Serial.print( lastUpdated );
    }
    
    Serial.print( ",\"" + GRAPHS_KEY + "\":[" );

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

Plotter::Graph::Graph( String title, VariableWrapper * wrappers, int size, bool xvy, int pointsDisplayed ) :
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
	Serial.print( "\"" + TITLE_KEY + "\":" ); Serial.print( "\"" + title + "\"" );
	Serial.print( ",\"" + XVY_KEY + "\":" ); Serial.print( xvy );
	Serial.print( ",\"" + POINTS_DISPLAYED_KEY + "\":" ); Serial.print( pointsDisplayed );
	Serial.print( ",\"" + SIZE_KEY + "\":" ); Serial.print( size );
	Serial.print( ",\"" + LABELS_KEY + "\":[" );
	for ( int i = 0; i < size; i++ )
	{
	    Serial.print( "\"" + wrappers[i].GetLabel() + "\"" );
	    if ( i + 1 < size )
	    {
		Serial.print( "," );
	    }
	}
	Serial.print( "],\"" + COLORS_KEY + "\":[" );
	for ( int i = 0; i < size; i++ )
	{
	    Serial.print( "\"" + wrappers[i].GetColor() + "\"" );
	    if ( i + 1 < size )
	    {
		Serial.print( "," );
	    }
	}
	Serial.print( "]," );
    }
    
    Serial.print( "\"" + DATA_KEY + "\":[" );
    for (int i = 0; i < size; i++)
    {
	Serial.print( wrappers[i].GetValue(), 8 );
	if ( i + 1 < size )
	{
	    Serial.print( "," );
	}
    }
    
    Serial.print( "]}" );
}

bool Plotter::Graph::SetColor( int sz, String * colors )
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

Plotter::VariableWrapper::VariableWrapper( String label, void * ref, double ( * deref )( void * ), String color ) :
    label( label ),
    color( color ),
    ref( ref ),
    deref( deref )
{}

String Plotter::VariableWrapper::GetLabel()
{
    return label;
}

double Plotter::VariableWrapper::GetValue()
{
    return deref( ref );
}

String Plotter::VariableWrapper::GetColor()
{
    return color;
}

void Plotter::VariableWrapper::SetColor( String col )
{
    color = col;
}
