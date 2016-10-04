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
  
  https://github.com/devinconley/ArduinoPlotter

  -------------------------------------------------------------------------------------------
  Plotter
  v1.1.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ===========================================================================================
 */

#include "Plotter.h"

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

/*
void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA) {
  String labels[] = {labelA};
  double* refs[] = {&refA};
  addGraphHelper(title, labels, refs, 1, false, points_displayed);
}
*/
/*
void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
			   String labelB, double & refB) {
  String labels[] = {labelA, labelB};
  double* refs[] = {&refA, &refB};
  addGraphHelper(title, labels, refs, 2, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
			   String labelB, double & refB, String labelC, double & refC) {
  String labels[] = {labelA, labelB, labelC};
  double* refs[] = {&refA, &refB, &refC};
  addGraphHelper(title, labels, refs, 3, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
			   String labelB, double & refB, String labelC, double & refC,
			   String labelD, double & refD) {
  String labels[] = {labelA, labelB, labelC, labelD};
  double* refs[] = {&refA, &refB, &refC, &refD};
  addGraphHelper(title, labels, refs, 4, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
			   String labelB, double & refB, String labelC, double & refC,
			   String labelD, double & refD, String labelE, double & refE) {
  String labels[] = {labelA, labelB, labelC, labelD, labelE};
  double* refs[] = {&refA, &refB, &refC, &refD, &refE};
  addGraphHelper(title, labels, refs, 5, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double & refA, 
			   String labelB, double & refB, String labelC, double & refC,
			   String labelD, double & refD, String labelE, double & refE,
			   String labelF, double & refF) {
  String labels[] = {labelA, labelB, labelC, labelD, labelE, labelF};
  double* refs[] = {&refA, &refB, &refC, &refD, &refE, &refF};
  addGraphHelper(title, labels, refs, 6, false, points_displayed);
}
  
void Plotter::addXYGraph(String title, int points_displayed, String labelX, double & refX, 
			 String labelY, double & refY) {
  String labels[] = {labelX, labelY};
  double* refs[] = {&refX, &refY};
  addGraphHelper(title, labels, refs, 2, true, points_displayed);
}
*/

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


Plotter::Graph::Graph(String title, VarWrapper * _wrappers, int size, bool xvy, int points_displayed) :
    title(title), size(size), xvy(xvy), points_displayed(points_displayed)
{
    next = NULL;
    wrappers = _wrappers;
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
