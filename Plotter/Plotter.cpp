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

Plotter::Plotter() {
  Serial.begin(115200);
  head = NULL;
  tail = NULL;
  total_size = 0;
  num_graphs = 0;
  max_points_displayed = 0;
  last_updated = millis();
}

Plotter::~Plotter()
{ 
    Graph * temp = head;
    Graph * temp_next;
    while (temp->next)
    {
	temp_next = temp->next;
	delete temp;
	temp = temp_next;
    }
    delete temp;
}

void Plotter::addGraphHelper(String title, VarWrapper * wrappers, int sz, bool xvy, int points_displayed) { 
    Graph * temp = new Graph( title, wrappers, sz, xvy, points_displayed );
    if (head)
    {
	tail->next = temp;
	tail = temp;
    }
    else
    {
	head = temp;
	tail = temp;
    }
  
    total_size += sz;
    num_graphs++;
    if (points_displayed > max_points_displayed)
    {
	max_points_displayed = points_displayed;
    }
  
    last_updated = millis();
}

bool Plotter::remove(int index) {
    if (num_graphs == 0 || index < 0 || num_graphs <= index)
    {
	return false;
    }
    else
    {
	Graph * temp = head;
	if (index == 0)
	{
	    head = head->next;
	    delete temp;
	}
	else
	{
	    Graph * last = temp;
	    for (int i = 0; i < index; i++)
	    {
		last = temp;
		temp = temp->next;
	    }
	    last->next = temp->next;
	    num_graphs--;
	    total_size -= temp->size;
	    delete temp;
	}
	last_updated = millis();
	return true;
    }
}

void Plotter::plot() {
    String code = OUTER_KEY;
    code += (num_graphs + INNER_KEY + total_size + INNER_KEY
	     + max_points_displayed + INNER_KEY + last_updated + INNER_KEY);
    Serial.print(code);
    Graph * temp = head;
    while (temp != NULL)
    {
	Serial.println();
	temp->plot();
	temp = temp->next;
    }
    Serial.println(OUTER_KEY);
}

// Graph

Plotter::Graph::Graph(String title, VarWrapper * wrappers, int size, bool xvy, int points_displayed) :
    title( title ),
    wrappers( wrappers ),
    size( size ),
    xvy( xvy ),
    points_displayed( points_displayed ),
    next( NULL )
{}

Plotter::Graph::~Graph()
{
    delete[] wrappers;
}

void Plotter::Graph::plot() {
    Serial.print(title); Serial.print(INNER_KEY);
    Serial.print(xvy); Serial.print(INNER_KEY);
    Serial.print(points_displayed); Serial.print(INNER_KEY);
    Serial.print(size); Serial.print(INNER_KEY);
    for (int i = 0; i < size; i++)
    {
	Serial.print( wrappers[i].GetLabel() ); Serial.print(INNER_KEY);
	Serial.print( wrappers[i].GetValue() ); Serial.print(INNER_KEY);
    }
}

// VariableWrapper

Plotter::VarWrapper::VarWrapper() :
    ref( NULL ),
    deref( NULL )
{}

Plotter::VarWrapper::VarWrapper( String label, void * ref, double ( * deref )( void * ) ) :
    label( label ),
    ref ( ref ),
    deref ( deref )
{}

String Plotter::VarWrapper::GetLabel()
{
    return label;
}

double Plotter::VarWrapper::GetValue()
{
    return deref( ref );
}
