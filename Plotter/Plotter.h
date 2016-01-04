#ifndef PLOTTER_H
#define PLOTTER_H

#include "Arduino.h"

class Plotter {
 public:
  Plotter();
  ~Plotter();
  void addTimeGraph(String title, String labelA, double* refA);
  void addTimeGraph(String title, String labelA, double* refA, String labelB, double* refB);
  void addTimeGraph(String title, String labelA, double* refA, String labelB, double* refB, 
		    String labelC, double* refC);
  
  void addXYGraph(String title, String labelX, double* refX, String labelY, double* refY);
  void init();
  void plot();


 private:
  // Internal helper class
  class GraphNode {
  public:
    GraphNode* next;
    GraphNode(String title, String* labels, double** refs, int size, bool xvy);
    void plot();
  private:
    bool xvy;
    int size;
    int points_displayed;
    String title;
    String* labels;
    double** refs;
  };
  // Private members
  int num_graphs;
  int total_size;
  unsigned long last_updated;
  GraphNode* head;
  
  static const char OUTER_KEY = '#';
  static const char INNER_KEY = '@';
};

#endif
