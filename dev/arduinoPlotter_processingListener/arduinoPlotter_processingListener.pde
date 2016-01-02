import processing.serial.*;
Serial port;


//CONSTANTS
final int[] COLORS = {#00FF00,#FF0000,#0000FF}; //int color codes

// Setup and config Globals
int h;
int w;
int sub_height;
int sub_width;
int num_graphs;
int total_vars;
String config_code = "This will not be matched!";
boolean configured = false;

// Graph-specific setup globals
int[][] pos_graphs; // stored as {x, width, y, height}
String[] titles;
String[] labels;
boolean[] xvy;
int[] min_var_index_graphs;
int[] sz_graphs;

// Data handling Globals
double[][] data;
double[] val_avg;
int[] weight_avg;
int[] num2avg;
int[] pos_x;

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
      plot_time(i);
    }
  } else {
    
  }
}

void plot_time(int graph_index) {
  int g = graph_index;
  fill(115);
  stroke(255);
  rect(pos_graphs[g][0],pos_graphs[g][1],sub_width,sub_height);
  
  int textPos = pos_graphs[g][1] + 30;
  int k = min_var_index_graphs[g];
  for (int i = 0; i < sz_graphs[g]; i++) {
      fill(COLORS[i]);
      text(labels[k + i],pos_graphs[g][0] + 10,textPos);
      textPos += 20;
      stroke(COLORS[i]);
      for (int j = pos_x[g]; j < sub_width; j++) {
	point((float)((pos_graphs[g][0] + sub_width) - (j-pos_x[g])), (float)((pos_graphs[g][1] + sub_height/2) - 5*data[j][k + i]));
      }
      for (int j = 0; j < pos_x[g]; j++) {
	point((float)((pos_graphs[g][0] + pos_x[g]) - j), (float)((pos_graphs[g][1] + sub_height/2) - 5*data[j][k + i]));
      }
    }
}

void serialEvent(Serial ser) {
  // Listen for serial data until #, the end of transmission key
  try {
    String temp = ser.readStringUntil('#');
    String[] array_main = temp.split("\n");
    
    // ********************************************************* //
    // ************* PLOT SETUP FROM CONFIG CODE *************** //
    // ********************************************************* //
    
    // If config code has changed, need to go through setup again
    if (!config_code.equals(array_main[0])) {
      configured = false;
      String[] array_sub = array_main[0].split("@");
      // Get number of graphs and determine orientation of each 
      num_graphs = Integer.parseInt(array_sub[0]);
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
      pos_graphs = new int[num_graphs][2];
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
      labels = new String[total_vars];
      titles = new String[num_graphs];
      xvy = new boolean[num_graphs];
      sz_graphs = new int[num_graphs];
      min_var_index_graphs = new int[num_graphs];
      data = new double[sub_width][total_vars];
      pos_x = new int[sub_width];
      val_avg = new double[total_vars];
      weight_avg = new int[num_graphs];
      num2avg = new int[num_graphs];
      
      // Iterate through the individual graph data blocks to get graph specific info
      k = 0; // k tracks overall variable index
      for (int i = 0; i < num_graphs; i++) {
	array_sub = array_main[i+1].split("@");
	titles[i] = array_sub[0];
	xvy[i] = Boolean.parseBoolean(array_sub[1]);
	num2avg[i] = round(Integer.parseInt(array_sub[2]) / sub_width);
	sz_graphs[i] = Integer.parseInt(array_sub[3]);
	min_var_index_graphs[i] = k;
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

    } else {
      // Matching a code means we have configured correctly
      configured = true;
     }

    // *********************************************************** //
    // ************ NORMAL PLOTTING FUNCTIONALITY **************** //
    // *********************************************************** //
    
    for (int i = 0; i < num_graphs; i++) {
      String[] array_sub = array_main[i+1].split("@");
      
      if (xvy[i]) {
	// Plot x vs y graph
	
      } else {
	// TIME GRAPH HANDLER
	int p = 5; // first index of double in split array
	// j is only a counter for var in graph context
	for (int j = min_var_index_graphs[i]; j < min_var_index_graphs[i] + sz_graphs[i]; j++) { 
	  // Update rolling average for all values associated with that graph
	  val_avg[j] = (weight_avg[i]*val_avg[j] + Double.parseDouble(array_sub[p])) 
	    / (weight_avg[i] + 1);
	  p += 2; // value will appear every 2 positions
	}
	weight_avg[i]++;
	
	// If enough values have been averaged for this graph, set vals appropriately
	if (weight_avg[i] >= num2avg[i]) {
	  for (int j = min_var_index_graphs[i]; j < min_var_index_graphs[i]+sz_graphs[i]; j++) {
	    data[pos_x[i]][j] = val_avg[j];
	  }
	  pos_x[i]++; // advance position of next placed value
	  if (pos_x[i] >= sub_width) {
	    pos_x[i] = 0;
	  }
	  weight_avg[i] = 0;
	}
      }
    }
    
  }
  catch (Exception e) {
    println("exception....");
  }
}