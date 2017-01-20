/*
  ===========================================================================================
  This listener is the source processing script that corresponds to the Arduino Plotter 
  library for Arduino. The library supports plots against time as well as 2-variable 
  "X vs Y" graphing. 
  -------------------------------------------------------------------------------------------
  The library transfers information via the serial port to a listener program written with the
  software provided by Processing. No modification is needed to this program; graph placement,
  axis-scaling, etc. are handled automatically. 
  Multiple options for this listener are available including stand-alone applications as well 
  as the source Processing script.

  The library, these listeners, a quick-start guide, and usage examples are available at:
  
  https://github.com/devinconley/Arduino-Plotter

  -------------------------------------------------------------------------------------------
  Arduino Plotter Listener
  v2.0.0
  https://github.com/devinconley/Arduino-Plotter
  by Devin Conley
  ===========================================================================================
*/

import processing.serial.*;
import java.util.Map;

//CONSTANTS
final char OUTER_KEY = '#';
final String INNER_KEY = "@";
final int MARGIN_SZ = 20; // between plots
final int BG_COL = 75; // background
final int PORT_INTERVAL = 5000; // time to sit on each port
final int BAUD_RATE = 115200;
final HashMap<String, Integer> COLORMAP = new HashMap<String, Integer>()
{
    {
	put( "red", color( 255, 0, 0 ) );
	put( "green", color( 0, 255, 0 ) );
	put( "blue", color( 0, 0, 255 ) );
	put( "orange", color( 255, 153, 51 ) );
	put( "yellow", color( 255, 255, 0 ) );
	put( "pink", color( 255, 51, 204 ) );
	put( "purple", color( 172, 0, 230 ) );
	put( "cyan", color( 0, 255, 255 ) );
    }
};

// Setup and config Globals
int h;
int w;
int numGraphs;
String configCode = "This will not be matched!";
boolean configured = false;
int lastConfig;
int lastPortSwitch;
int portIndex;
Serial port;
ArrayList<Graph> graphs;

void setup()
{
    // Canvas
    size( 800, 800 );
    surface.setResizable( true );
    h = height;
    w = width;
    frameRate( 100 );
    
    // Serial comms
    portIndex = 0;
    lastPortSwitch = millis();
    attemptConnect( portIndex );
}

void draw()
{
    //PLOT ALL
    try
    {
	background( BG_COL );
	
	if ( configured )
	{	    
	    for( int i = 0; i < graphs.size(); i++ )
	    {
		graphs.get(i).Plot();
	    }
	}
	else
	{
	    text( "Scanning serial ports... (Port " + portIndex + ")", 20, 20 );
	    // Continue to scan ports if not configuring
	    if ( millis() - lastPortSwitch > PORT_INTERVAL )
	    {	// Go to next port
		portIndex++;
		if ( portIndex >= Serial.list().length )
		{
		    portIndex = 0;
		}
		attemptConnect( portIndex );
	    }
	}
	// Resize if needed
	if ( h != height || w != width)
	{
	    h = height;
	    w = width;
	    float[][] posGraphs = setupGraphPosition( numGraphs );
	    for ( int i = 0; i < numGraphs; i++ )
	    {
		graphs.get(i).Reconfigure( posGraphs[i][0], posGraphs[i][1], posGraphs[i][2], posGraphs[i][3] );
	    }
	}
    }
    catch ( Exception e )
    {}
}

void serialEvent( Serial ser )
{
    // Listen for serial data until #, the end of transmission key
    try
    {
	String message = ser.readStringUntil( OUTER_KEY );
	String[] arrayMain = message.split( "\n" );
    
	// ********************************************************* //
	// ************* PLOT SETUP FROM CONFIG CODE *************** //
	// ********************************************************* //
    
	// If config code has changed, need to go through setup again
	if ( !configCode.equals( arrayMain[0] ) )
	{
	    String[] arraySub = arrayMain[0].split( INNER_KEY );
	    // Check for size of full transmission against expected to flag bad transmission
	    numGraphs = Integer.parseInt( arraySub[0] );
	    if ( arrayMain.length != numGraphs + 1 )
	    {
		throw new Exception();
	    }
	    configured = false;
	    //print("New config");
            
	    // Setup new layout
	    float[][] posGraphs = setupGraphPosition( numGraphs );
      
	    graphs = new ArrayList<Graph>();
      
	    // Iterate through the individual graph data blocks to get graph specific info
	    for ( int i = 0; i < numGraphs; i++ )
	    {
		arraySub = arrayMain[i+1].split( INNER_KEY );
		String title = arraySub[0];
		boolean xvyTemp = Integer.parseInt( arraySub[1] ) == 1;
		int maxPoints = Integer.parseInt( arraySub[2] );
		int numVars = Integer.parseInt( arraySub[3] );
		String[] labelsTemp = new String[numVars];
		int[] colorsTemp = new int[numVars];
       
		for ( int j = 0; j < numVars; j++ )
		{
		    labelsTemp[j] = arraySub[4 + 3*j];
		    colorsTemp[j] = COLORMAP.get( arraySub[5 + 3*j] );
		    if ( colorsTemp[j] == 0 )
		    {
			colorsTemp[j] = COLORMAP.get( "green" );
		    }
		    println( colorsTemp[j] );
		}

		if ( xvyTemp )
		{
		    numVars = 1;
		}
		
		// Create new Graph
		Graph temp = new Graph( this, posGraphs[i][0], posGraphs[i][1], posGraphs[i][2], posGraphs[i][3],
					xvyTemp, numVars, maxPoints, title, labelsTemp, colorsTemp );
		graphs.add( temp );
	    }
	    println("Added ", graphs.size() ); 
      
	    // Set new config code
	    configCode = arrayMain[0];
	    //println(config_code);
	    lastConfig = millis();
	}
	else
	{
	    // Matching a code means we have configured correctly
	    configured = true;
          
	    // *********************************************************** //
	    // ************ NORMAL PLOTTING FUNCTIONALITY **************** //
	    // *********************************************************** //
	    int tempTime = millis();
      
	    for ( int i = 0; i < numGraphs; i++ )
	    {
		String[] arraySub = arrayMain[i+1].split( INNER_KEY );

		double[] tempData = new double[ (arraySub.length - 5) / 3 ];
		
		// Update graph objects with new data
		int q = 0;
		for ( int j = 6; j < arraySub.length; j += 3 )
		{
		    tempData[q] = Double.parseDouble( arraySub[j] );
		    q++;       
		}
		graphs.get( i ).Update( tempData, tempTime );		
	    }
	}
    }
    catch (Exception e) {
	//println("exception....");
    }
}

// Helper method to calculate bounds of graphs
float[][] setupGraphPosition( int numGraphs )
{
    // Determine orientation of each graph      
    int numHigh = 1;
    int numWide = 1;
    // Increase num subsections in each direction until all graphs can fit
    while ( numHigh * numWide < numGraphs )
    {
	if ( numWide > numHigh )
	{
	    numHigh++;
	}
	else if ( numHigh > numWide )
	{
	    numWide++;
	}
	else if ( height >= width )
	{
	    numHigh++;
	}
	else
	{
	    // Want to increase in width first
	    numWide++;
	}	
    }

    float[][] posGraphs = new float[numGraphs][4];

    float subHeight = round( h / numHigh );
    float subWidth = round( w / numWide );
    
    // Set bounding box for each subsection
    for(int i = 0; i < numHigh; i++)
    {
	for (int j = 0; j < numWide; j++)
	{
	    int k = i * numWide + j;
	    if ( k < numGraphs )
	    {
		posGraphs[k][0] = i*subHeight + MARGIN_SZ / 2;
		posGraphs[k][1] = j*subWidth + MARGIN_SZ / 2;
		posGraphs[k][2] = subHeight - MARGIN_SZ;
		posGraphs[k][3] = subWidth - MARGIN_SZ;
	    }
	}
    }

    return posGraphs;
}

void attemptConnect( int index )
{
    // Attempt connect on specified serial port
    try
    {
	// Configure
	port = new Serial( this, Serial.list()[portIndex], BAUD_RATE );  
	port.bufferUntil( OUTER_KEY );
	lastPortSwitch = millis(); // at end so that we try again immediately on invalid port
    }
    catch ( Exception e )
    {}
}