import processing.serial.*;
Serial port;


//CONSTANTS
final int H = 1150;
final int W = 1600;

final int XSCALE = 5; //inverse (5 => .2). Using 1 will mean no compression
final int FRAMERATE = 50000;
final int NUMVARS = 44;
final String[] labels = {"ax Raw", "ay Raw", "az Raw","ax Filtered", "ay Filtered", "az Filtered","ax adj","ay adj","az adj", "ax final","ay final", "az final", "vx unfilt", "vy unfilt", "vz unfilt", "x velocity", "y velocity", "z velocity", "x position unfilt", "y position unfilt", "z position unfilt","x position final","y position final","z position final", "gx raw", "gy raw", "gz raw", "gx Filtered", "gy Filtered", "gz Filtered", "gx final", "gy final", "gz final", "xo by G","yo by G","zo by G","x-o by Accel","y-o by Accel","z-o by Accel","x-orientation", "y-orientation","z-orienation","Distance Threshold","Velocity Threshold"};
final int[] YSCALE = {5,5,5,5,5,5,5,5,5,5,5,5,50,50,50,50,50,50,10,10,10,10,10,10,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,50,50};

//PLOT PRESETS
final int[] accelRaw = {0,1,2};
final int[] accelFilt = {3,4,5};
final int[] accel = {9,10,11}; // (rotated)
final int[] gyroRaw = {22,23,24};
final int[] gyro = {27,28,29};
final int[] orByGyro = {33,34,35};
final int[] orByAccel = {36,37,38};
final int[] orientation = {39,40,41};
final int[] velRaw = {12,13,14};
final int[] velocity = {15,16,17};
final int[] position = {21,22,23};

final int[] colors = {#00FF00,#FF0000,#0000FF}; //int color codes
//Globals
String temp;
int posX = 0;
int iVal = 0;
float[][] data = new float[W][NUMVARS+2];
float[] val = new float[NUMVARS];

void setup() {
  size(W, H, P2D);
  String portID = Serial.list()[0];
  port = new Serial(this, portID, 115200);  
  port.bufferUntil('x');
  frameRate(FRAMERATE);
  textSize(16);
}

void draw() {
    //PLOT ALL
    background(155);
    plot(orientation,-400);
    plot(accel, 250);
    plot(velocity, 250);
    plotDistThresh(300);
    //plot(position, 250);

}

void plot(int[] indices, int offset) {
  translate(0, offset);
  stroke(255);
  line(0,H/2,W,H/2);
  int textPos = H/2 - 100;
    for (int j = 0; j < indices.length; j++) {
      fill(colors[j]);
      text(labels[indices[j]],10,textPos);
      textPos += 20;
      stroke(colors[j]);
      for (int i = posX; i < W; i++) {
	point(W - (i-posX), H/2 - YSCALE[indices[j]]*data[i][indices[j]]);
      }
      for (int i = 0; i < posX; i++) {
	point(posX - i, H/2 - YSCALE[indices[j]]*data[i][indices[j]]);
      }
    }
}

void plotDistThresh(int offset) {
  translate(0, offset);
  stroke(255);
  line(0,H/2,W,H/2);
  stroke(colors[1]);
  line(0,H/2 - YSCALE[NUMVARS-1]*data[0][NUMVARS-1],W,H/2 - YSCALE[NUMVARS-1]*data[0][NUMVARS-1]);
  int textPos = H/2 - 200;
  fill(colors[1]);
  text("Distance Threshold",10,textPos);
  textPos += 20;
  fill(colors[2]);
  stroke(colors[2]);
  text("Current Distance",10,textPos);
  for (int i = posX; i < W; i++) {
    point(W - (i-posX), H/2 -  YSCALE[NUMVARS-1]*data[i][NUMVARS+1]);
  }
  for (int i = 0; i < posX; i++) {
    point(posX - i, H/2 - YSCALE[NUMVARS-1]*data[i][NUMVARS+1]);
  }
}

void serialEvent(Serial p) {

  try {
    temp = p.readStringUntil('x');
    String[] array = temp.split("\n");

    //Running average for values (unless XSCALE == 1)
    for (int i = 0; i < NUMVARS; i++) {
      val[i] = (iVal*val[i] +  Float.parseFloat(array[i]))/(iVal+1);
    }
    iVal++;
    if (iVal >=XSCALE) {
      for (int i = 0; i < NUMVARS; i++) {
	data[posX][i] = val[i];
      }
      data[posX][NUMVARS] = sqrt(sq(data[posX][15])+sq(data[posX][16])+sq(data[posX][17]));
      data[posX][NUMVARS+1] = sqrt(sq(data[posX][21])+sq(data[posX][22])+sq(data[posX][23]));
      posX++;
      if (posX >= W) {
	posX = 0;
      }
      iVal = 0;
    }
  }
  catch (Exception e) {
    println("exception....");
  }
}