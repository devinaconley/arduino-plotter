/*
  ===========================================================================================
  Utilities and constants
  -------------------------------------------------------------------------------------------
  Plotter
  v2.4.1
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

#ifndef UTIL_H
#define UTIL_H

// Constants
static const int CONFIG_INTERVAL = 50;
static const char* COLORS[] = { "green", "orange", "cyan", "yellow", "pink", "blue" };

// Transmission Keys
static const char* OUTER_KEY = "#";
static const char* TIME_KEY = "t";
static const char* NUM_GRAPH_KEY = "ng";
static const char* LAST_UPDATED_KEY = "lu";
static const char* GRAPHS_KEY = "g";
static const char* TITLE_KEY = "t";
static const char* XVY_KEY = "xvy";
static const char* POINTS_DISPLAYED_KEY = "pd";
static const char* SIZE_KEY = "sz";
static const char* LABELS_KEY = "l";
static const char* COLORS_KEY = "c";
static const char* DATA_KEY = "d";

#endif