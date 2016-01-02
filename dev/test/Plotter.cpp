#include "Plotter.h"

Plotter::Plotter() {
  Serial.begin(115200);
  head = NULL;
  total_size = 0;
  num_graphs = 0;
  last_updated = millis();
}

Plotter::~Plotter() { }


void Plotter::addTimeGraph(String title, String labelA, double* refA) {
  GraphNode* temp = head;
  String labels[] = {labelA};
  double* refs[] = {refA};
  head = new GraphNode(title, labels, refs, 1, false);
  head->next = temp;
  total_size++;
  num_graphs++;
  last_updated = millis();
}

void Plotter::addTimeGraph(String title, String labelA, double* refA, 
			   String labelB, double* refB) {
  GraphNode* temp = head;
  String labels[] = {labelA, labelB};
  double* refs[] = {refA, refB};
  head = new GraphNode(title, labels, refs, 2, false);
  head->next = temp;
  total_size+=2;
  num_graphs++;
  last_updated = millis();
}

void Plotter::addTimeGraph(String title, String labelA, double* refA, 
			   String labelB, double* refB, String labelC, double* refC) {
  GraphNode* temp = head;
  String labels[] = {labelA, labelB, labelC};
  double* refs[] = {refA, refB, refC};
  head = new GraphNode(title, labels, refs, 3, false);
  head->next = temp;
  total_size+=3;
  num_graphs++;
  last_updated = millis();
}


void Plotter::addXYGraph(String title, String labelX, double* refX, String labelY, double* refY) {
  total_size+=2;
  num_graphs++;
  last_updated = millis();
}

void Plotter::init() {

}

void Plotter::plot() {
  // Print configuration code
  Serial.print(num_graphs); Serial.print('@'); Serial.print(total_size); Serial.print('@'); 
  Serial.print(last_updated); Serial.print('@'); Serial.println();
  GraphNode* temp = head;
  while (temp != NULL) {
    temp->plot();
    temp = temp->next;
  }
  Serial.print('#');
}

// Functions for helper class
Plotter::GraphNode::GraphNode(String title, String* _labels, double** _refs, int size, bool xvy) :  title(title), size(size), xvy(xvy) {
  next = NULL;
  points_displayed = 300;
  labels = new String[size];
  refs = new double*[size];
  for (int i = 0; i < size; i++) {
    labels[i] = _labels[i];
    refs[i] = _refs[i];
  }
}

void Plotter::GraphNode::plot() {
  Serial.print(title); Serial.print('@');
  Serial.print(true); Serial.print('@');
  Serial.print(points_displayed); Serial.print('@');
  Serial.print(size); Serial.print('@');
  for (int i = 0; i < size; i++) {
    Serial.print(labels[i]); Serial.print('@');
    Serial.print(*refs[i]); Serial.print('@');
  }
  Serial.println();
}
