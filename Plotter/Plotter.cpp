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

#include "Plotter.h"

// Plotter

Plotter::Plotter()
{
  Serial.begin(115200);
  head = NULL;
  tail = NULL;
  totalSize = 0;
  numGraphs = 0;
  maxPointsDisplayed = 0;
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
  
    totalSize += sz;
    numGraphs++;
    if ( pointsDisplayed > maxPointsDisplayed )
    {
	maxPointsDisplayed = pointsDisplayed;
    }
  
    lastUpdated = millis();
}

bool Plotter::remove(int index)
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
	    totalSize -= temp->size;
	    delete temp;
	}
	lastUpdated = millis();
	return true;
    }
}

void Plotter::Plot()
{
    String code = OUTER_KEY;
    code += ( numGraphs + INNER_KEY + totalSize + INNER_KEY
	     + maxPointsDisplayed + INNER_KEY + lastUpdated + INNER_KEY );
    Serial.print( code );
    Graph * temp = head;
    while ( temp != NULL )
    {
	Serial.println();
	temp->Plot();
	temp = temp->next;
    }
    Serial.println( OUTER_KEY );
}

// Graph

Plotter::Graph::Graph( String title, VariableWrapper * wrappers, int size, bool xvy, int pointsDisplayed ) :
    title( title ),
    wrappers( wrappers ),
    size( size ),
    xvy( xvy ),
    pointsDisplayed( pointsDisplayed ),
    next( NULL )
{}

Plotter::Graph::~Graph()
{
    delete[] wrappers;
}

void Plotter::Graph::Plot()
{
    Serial.print( title ); Serial.print( INNER_KEY );
    Serial.print( xvy ); Serial.print( INNER_KEY );
    Serial.print( pointsDisplayed ); Serial.print( INNER_KEY );
    Serial.print( size ); Serial.print( INNER_KEY );
    for (int i = 0; i < size; i++)
    {
	Serial.print( wrappers[i].GetLabel() ); Serial.print( INNER_KEY );
	Serial.print( wrappers[i].GetValue() ); Serial.print( INNER_KEY );
    }
}

// VariableWrapper

Plotter::VariableWrapper::VariableWrapper() :
    ref( NULL ),
    deref( NULL )
{}

Plotter::VariableWrapper::VariableWrapper( String label, void * ref, double ( * deref )( void * ) ) :
    label( label ),
    ref ( ref ),
    deref ( deref )
{}

String Plotter::VariableWrapper::GetLabel()
{
    return label;
}

double Plotter::VariableWrapper::GetValue()
{
    return deref( ref );
}
