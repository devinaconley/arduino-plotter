import processing.serial.*;
Serial port;

//CONSTANTS
final int[] COLORS = {#00FF00,#FF0000,#0000FF}; //int color codes
final char OUTER_KEY = '#';
final String INNER_KEY = "@";
final double Y_SCALE_COVERAGE = 0.75;

// Setup and config Globals
int h;
int w;
float sub_height;
float sub_width;
int num_graphs;
int total_vars;
int max_points;
String config_code = "This will not be matched!";
boolean configured = false;
int last_config;

// Graph-specific setup globals
float[][] pos_graphs; // stored as {x, y}
String[] titles;
boolean[] xvy;
int[] first_index_graphs;
int[] sz_graphs;
int[] num_points;
double[][] extremes_graphs; // {min_x, max_x, min_y, max_y}
int[] pos_x;

// Variable-specific Globals
double[][][] data;
String[] labels;


void setup() {
  h = displayHeight;
  w = displayWidth;
  fullScreen();
  String portID = Serial.list()[0];
  port = new Serial(this, portID, 115200);  
  port.bufferUntil('#');
  frameRate(20);
  textSize(16);
  background(75);
}

void draw() {
  //PLOT ALL
  if (configured) {
    background(75);
    for (int i = 0; i < num_graphs; i++) {
      if (xvy[i]) {
	plot_xy(i);
      } else{
	plot_time(i);
      }
    }
  } else {
    
  }
}

void plot_xy(int graph_index) {
  int g = graph_index;
  int k = first_index_graphs[g];   

  // Calculations for offset and scaling of graph
  double x_scale = sub_width / (extremes_graphs[g][1] - extremes_graphs[g][0]);
  double x_offset = x_scale*extremes_graphs[g][0];
  double y_scale = Y_SCALE_COVERAGE*sub_height / (extremes_graphs[g][3] - extremes_graphs[g][2]);
  double y_offset = y_scale*extremes_graphs[g][3] + 0.5*(1.0 - Y_SCALE_COVERAGE)*sub_height;
  
  // Reset the extremes to the first value in data and get min and max again
  extremes_graphs[g][0] = data[0][k][1];
  extremes_graphs[g][1] = data[0][k][1];
  extremes_graphs[g][2] = data[0][k+1][1];
  extremes_graphs[g][3] = data[0][k+1][1];

  // Drawing setup
  fill(115);
  stroke(255);
  rect(pos_graphs[g][0],pos_graphs[g][1],sub_width,sub_height);
  
  // ** add support for multiple paths in x-y here **
  stroke(COLORS[0]);
  for (int j = 0; j < num_points[g]; j++) {
    point( (float)(pos_graphs[g][0] + (data[j][k][1]*x_scale - x_offset)),
	   (float)(pos_graphs[g][1] + y_offset - data[j][k+1][1]*y_scale) );
    // Check for min or max
    if (data[j][k][1] < extremes_graphs[g][0]) {
      extremes_graphs[g][0] = data[j][k][1];
    } else if (data[j][k][1] > extremes_graphs[g][1]) {
      extremes_graphs[g][1] = data[j][k][1];
    }
    if (data[j][k+1][1] < extremes_graphs[g][2]) {
      extremes_graphs[g][2] = data[j][k+1][1];
    } else if (data[j][k+1][1] > extremes_graphs[g][3]) {
      extremes_graphs[g][3] = data[j][k+1][1];
    }	
  }

}

void plot_time(int graph_index) {
  int g = graph_index;
  int k = first_index_graphs[g];   

  // Calculations for offset and scaling of graph
  double x_scale = sub_width / (extremes_graphs[g][1] - extremes_graphs[g][0]);
  double x_offset = x_scale*extremes_graphs[g][0];
  double y_scale = Y_SCALE_COVERAGE*sub_height / (extremes_graphs[g][3] - extremes_graphs[g][2]);
  double y_offset = y_scale*extremes_graphs[g][3] + 0.5*(1.0 - Y_SCALE_COVERAGE)*sub_height;

  // Reset the y-extremes to the first value in data and get min and max again
  extremes_graphs[g][2] = data[0][k][1];
  extremes_graphs[g][3] = data[0][k][1];
  
  // Drawing setup
  fill(115);
  stroke(255);
  rect(pos_graphs[g][0],pos_graphs[g][1],sub_width,sub_height);
  float textPos = pos_graphs[g][1] + 30;
  // Plot each line
  for (int i = 0; i < sz_graphs[g]; i++) {
      fill(COLORS[i]);
      text(labels[k + i],pos_graphs[g][0] + 10,textPos);
      textPos += 20;
      stroke(COLORS[i]);
      
      for (int j = 0; j < num_points[g]; j++) {
	point( (float)(pos_graphs[g][0] + (data[j][k+i][0]*x_scale - x_offset)),
	       (float)(pos_graphs[g][1] + y_offset - data[j][k+i][1]*y_scale) );
	// Check for min or max
	if (data[j][k+i][1] < extremes_graphs[g][2]) {
	  extremes_graphs[g][2] = data[j][k+i][1];
	} else if (data[j][k+i][1] > extremes_graphs[g][3]) {
	  extremes_graphs[g][3] = data[j][k+i][1];
	}
	
      }
      
    }
}

void serialEvent(Serial ser) {
  // Listen for serial data until #, the end of transmission key
  try {
    String temp = ser.readStringUntil(OUTER_KEY);
    String[] array_main = temp.split("\n");
    
    // ********************************************************* //
    // ************* PLOT SETUP FROM CONFIG CODE *************** //
    // ********************************************************* //
    
    // If config code has changed, need to go through setup again
    if (!config_code.equals(array_main[0])) {
      String[] array_sub = array_main[0].split(INNER_KEY);
      // Check for size of full transmission against expected to flag bad transmission
      num_graphs = Integer.parseInt(array_sub[0]);
      if (array_main.length != num_graphs+1) {
	throw new Exception();
      }
      // Determine orientation of each graph      
      int num_high = 1;
      int num_wide = 1;
      // Increase num subsections in each direction until all graphs can fit
      while (num_high * num_wide < num_graphs) {
	if (num_wide > num_high) {
	  num_high++;
	} else {
	  // Want to increase in width first
	  num_wide++;
	}	
      }
      
      // Set bounding box for each subsection
      pos_graphs = new float[num_graphs][2];
      int k = 0; // k tracks overall graph index
      sub_width = round(w/num_wide);
      sub_height = round(h/num_high);
      for (int i = 0; i < num_high; i++) {
	for (int j = 0; j < num_wide; j++) {
	  if (k < num_graphs) {
	    pos_graphs[k][0] = j*sub_width;
	    pos_graphs[k][1] = i*sub_height;
	  }
	  k++;
	}
      }

      // Reset data storage arrays
      total_vars = Integer.parseInt(array_sub[1]);
      max_points = Integer.parseInt(array_sub[2]);
      labels = new String[total_vars];
      titles = new String[num_graphs];
      xvy = new boolean[num_graphs];
      sz_graphs = new int[num_graphs];
      extremes_graphs = new double[num_graphs][4];
      first_index_graphs = new int[num_graphs];
      data = new double[max_points][total_vars][2];
      
      pos_x = new int[max_points];
      num_points = new int[num_graphs];
      
      // Iterate through the individual graph data blocks to get graph specific info
      k = 0; // k tracks overall variable index
      for (int i = 0; i < num_graphs; i++) {
	array_sub = array_main[i+1].split(INNER_KEY);
	titles[i] = array_sub[0];
	xvy[i] = Integer.parseInt(array_sub[1]) == 1;
	println(xvy[i]);
	num_points[i] = Integer.parseInt(array_sub[2]);
	sz_graphs[i] = Integer.parseInt(array_sub[3]);
	first_index_graphs[i] = k;
	int p = 4; // first label of this graph falls at index 4
	for (int j = 0; j < sz_graphs[i]; j++) {
	  labels[k] = array_sub[p];
	  p += 2;
	  k++;
	}
      }
      // Set new config code
      config_code = array_main[0];
      println(config_code);
      last_config = millis();
    } else {
      // Matching a code means we have configured correctly
      configured = true;
    
      
      // *********************************************************** //
      // ************ NORMAL PLOTTING FUNCTIONALITY **************** //
      // *********************************************************** //
      int temp_time = millis();
      
      for (int i = 0; i < num_graphs; i++) {
	String[] array_sub = array_main[i+1].split(INNER_KEY);

	if (xvy[i]) {
	  // Plot x vs y graph
	  int p = 5; // first index of double in split array
	  
	  // j is only a counter for var in graph context
	  for (int j = first_index_graphs[i]; j < first_index_graphs[i] + sz_graphs[i]; j++) { 
	    data[pos_x[i]][j][1] = Double.parseDouble(array_sub[p]);
	    p += 2;	    
	  }

	  // Advance pos_x and rollback pos_x if exceeds max points for specific graph
	  pos_x[i]++;
	  if (pos_x[i] >= num_points[i]) {
	    pos_x[i] = 0;	  
	  }
	  
	} else {
	  // TIME GRAPH HANDLER
	  int p = 5; // first index of double in split array
	  
	  // j is only a counter for var in graph context
	  for (int j = first_index_graphs[i]; j < first_index_graphs[i] + sz_graphs[i]; j++) { 
	    data[pos_x[i]][j][0] = temp_time;
	    data[pos_x[i]][j][1] = Double.parseDouble(array_sub[p]);
	    p += 2;
	  }
	  // Max timestamp of graph will be now
	  extremes_graphs[i][1] = temp_time;
	  
	  // Advance pos_x and rollback pos_x if exceeds max points for specific graph
	  pos_x[i]++;
	  if (pos_x[i] >= num_points[i]) {
	    pos_x[i] = 0;	  
	  }
	  
	  // Min timestamp will be whatever this new pos_x has (about to be replaced)
	  if (data[pos_x[i]][first_index_graphs[i]][0] != 0) {
	    extremes_graphs[i][0] = data[pos_x[i]][first_index_graphs[i]][0];
	  } else { 
	    // But if hasn't fully filled array, need to estimate
	    extremes_graphs[i][0] = temp_time - 
	      ((temp_time - last_config) / pos_x[i])*num_points[i];
	  }
	  	  
	}
      }
    }
  }
  catch (Exception e) {
    //println("exception....");
  }
}