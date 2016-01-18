/*
  ///////////////////////////////////////////////////////////////////////////////////////////
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
  v1.0.0
  https://github.com/devinconley/ArduinoPlotter
  by Devin Conley
  ///////////////////////////////////////////////////////////////////////////////////////////
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

Plotter::~Plotter() { 
  GraphNode* temp = head;
  GraphNode* temp_next;
  while (temp->next) {
    temp_next = temp->next;
    delete temp;
    temp = temp_next;
  }
  delete temp;
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA) {
  String labels[] = {labelA};
  double* refs[] = {refA};
  addGraphHelper(title, labels, refs, 1, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB) {
  String labels[] = {labelA, labelB};
  double* refs[] = {refA, refB};
  addGraphHelper(title, labels, refs, 2, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC) {
  String labels[] = {labelA, labelB, labelC};
  double* refs[] = {refA, refB, refC};
  addGraphHelper(title, labels, refs, 2, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC,
			   String labelD, double* refD) {
  String labels[] = {labelA, labelB, labelC, labelD};
  double* refs[] = {refA, refB, refC, refD};
  addGraphHelper(title, labels, refs, 4, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC,
			   String labelD, double* refD, String labelE, double* refE) {
  String labels[] = {labelA, labelB, labelC, labelD, labelE};
  double* refs[] = {refA, refB, refC, refD, refE};
  addGraphHelper(title, labels, refs, 5, false, points_displayed);
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC,
			   String labelD, double* refD, String labelE, double* refE,
			   String labelF, double* refF) {
  String labels[] = {labelA, labelB, labelC, labelD, labelE, labelF};
  double* refs[] = {refA, refB, refC, refD, refE, refF};
  addGraphHelper(title, labels, refs, 6, false, points_displayed);
}
  
void Plotter::addXYGraph(String title, int points_displayed, String labelX, double* refX, 
			 String labelY, double* refY) {
  String labels[] = {labelX, labelY};
  double* refs[] = {refX, refY};
  addGraphHelper(title, labels, refs, 2, true, points_displayed);
}

void Plotter::addGraphHelper(String title, String labels[], double* refs[], int sz, bool xvy, int points_displayed) { 
  GraphNode* temp = new GraphNode(title, labels, refs, sz, xvy, points_displayed);
  if (head) {
    tail->next = temp;
    tail = temp;
  } else {
    head = temp;
    tail = temp;
  }
  
  total_size += sz;
  num_graphs++;
  if (points_displayed > max_points_displayed) {
    max_points_displayed = points_displayed;
  }
  last_updated = millis();
}


void Plotter::plot() {
  // Print configuration code
  String code = OUTER_KEY;
  code += (num_graphs + INNER_KEY + total_size + INNER_KEY
	   + max_points_displayed + INNER_KEY + last_updated + INNER_KEY);
  Serial.print(code);
  // Print each graph code
  GraphNode* temp = head;
  while (temp != NULL) {
    Serial.println();
    temp->plot();
    temp = temp->next;
  }
  Serial.println(OUTER_KEY);
}

// Functions for helper class
Plotter::GraphNode::GraphNode(String title, String* _labels, double** _refs, int size, bool xvy, 
			      int points_displayed) : title(title), size(size), xvy(xvy), 
						      points_displayed(points_displayed) {
  next = NULL;
  labels = new String[size];
  refs = new double*[size];
  for (int i = 0; i < size; i++) {
    labels[i] = _labels[i];
    refs[i] = _refs[i];
  }
}

void Plotter::GraphNode::plot() {
  Serial.print(title); Serial.print('@');
  Serial.print(xvy); Serial.print('@');
  Serial.print(points_displayed); Serial.print('@');
  Serial.print(size); Serial.print('@');
  for (int i = 0; i < size; i++) {
    Serial.print(labels[i]); Serial.print('@');
    Serial.print(*refs[i]); Serial.print('@');
  }
}
