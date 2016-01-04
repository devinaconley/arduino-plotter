#include "Plotter.h"

Plotter::Plotter() {
  Serial.begin(115200);
  head = NULL;
  total_size = 0;
  num_graphs = 0;
  max_points_displayed = 0;
  last_updated = millis();
}

Plotter::~Plotter() { }


void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA) {
  GraphNode* temp = head;
  String labels[] = {labelA};
  double* refs[] = {refA};
  head = new GraphNode(title, labels, refs, 1, false, points_displayed);
  head->next = temp;
  total_size++;
  num_graphs++;
  if (points_displayed > max_points_displayed) {
    max_points_displayed = points_displayed;
  }
  last_updated = millis();
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB) {
  GraphNode* temp = head;
  String labels[] = {labelA, labelB};
  double* refs[] = {refA, refB};
  head = new GraphNode(title, labels, refs, 2, false, points_displayed);
  head->next = temp;
  total_size+=2;
  num_graphs++;
  if (points_displayed > max_points_displayed) {
    max_points_displayed = points_displayed;
  }
  last_updated = millis();
}

void Plotter::addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC) {
  GraphNode* temp = head;
  String labels[] = {labelA, labelB, labelC};
  double* refs[] = {refA, refB, refC};
  head = new GraphNode(title, labels, refs, 3, false, points_displayed);
  head->next = temp;
  total_size+=3;
  num_graphs++;
  if (points_displayed > max_points_displayed) {
    max_points_displayed = points_displayed;
  }
  last_updated = millis();
}


void Plotter::addXYGraph(String title, int points_displayed, String labelX, double* refX, String labelY, double* refY) {
  // Code for creating XYgraph...
  GraphNode* temp = head;
  String labels[] = {labelX, labelY};
  double* refs[] = {refX, refY};
  head = new GraphNode(title, labels, refs, 2, true, points_displayed);
  head->next = temp;
  total_size+=2;
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
  /*
    Serial.print(OUTER_KEY);
  Serial.print(num_graphs); Serial.print('@'); Serial.print(num_time); Serial.print(INNER_KEY);
  Serial.print(total_size); Serial.print('@'); 
  Serial.print(last_updated); Serial.print('@'); Serial.println();
  */
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
