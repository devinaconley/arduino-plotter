#ifndef PLOTTER_H
#define PLOTTER_H

#include "Arduino.h"

class Plotter {
 public:
  Plotter();
  ~Plotter();
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA);
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
		    String labelB, double* refB);
  void addTimeGraph(String title, int points_displayed, String labelA, double* refA, 
		    String labelB, double* refB, String labelC, double* refC);
  
  void addXYGraph(String title, int points_displayed,
		  String labelX, double* refX, String labelY, double* refY);
  void init();
  void plot();


 private:
  // Internal helper class
  class GraphNode {
  public:
    GraphNode* next;
    GraphNode(String title, String* labels, double** refs, int size, bool xvy, 
	      int points_displayed);
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
  int max_points_displayed;
  unsigned long last_updated;
  GraphNode* head;
  
};

static const String OUTER_KEY = "#";
static const String INNER_KEY = "@";


#endif
