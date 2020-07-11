/*
  ===========================================================================================
  This listener is the main processing script that corresponds to the Arduino Plotter 
  library for Arduino. This driver script handles serial port information and manages a 
  set of Graph objects to do the actual plotting. 
  -------------------------------------------------------------------------------------------
  The library stores and handles all relevant graph information and variable references, 
  and transfers information via the serial port to a listener program written with the
  software provided by Processing. No modification is needed to this program; graph placement,
  axis-scaling, etc. are handled automatically. 
  Multiple options for this listener are available including stand-alone applications as well 
  as the source Processing script.

  The library, these listeners, a quick-start guide, documentation, and usage examples are 
  available at:
  
  https://github.com/devinaconley/arduino-plotter

  -------------------------------------------------------------------------------------------
  Arduino Plotter Listener
  v2.4.1
  https://github.com/devinaconley/arduino-plotter
  by Devin Conley
  ===========================================================================================
*/

import processing.serial.*;
import java.util.Map;

import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.TimeoutException;

// FLAG FOR DEBUG MODE
final boolean DEBUG = false;

//CONSTANTS
final char OUTER_KEY = '#';
final int MARGIN_SZ = 20; // between plots
final int BG_COL = 75; // background
final int PORT_INTERVAL = 5000; // time to sit on each port
final int CONNECT_TIMEOUT = 2000; // force timeout on connecting to serial port
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
String lastLabels = "Also will not be matched";
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
    while ( Serial.list().length < 1 )
    {
	text( "No serial ports available. Waiting...", 20, 20 );	
	delay( 100 );
    }
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
            // Continue to scan ports if not configuring
	    text( "Scanning serial ports... (" + Serial.list()[portIndex] + ")", 20, 20 );
	    
	    if ( millis() - lastPortSwitch > PORT_INTERVAL )
	    {	// Go to next port		
		portIndex++;
		if ( portIndex >= Serial.list().length )
		{
		    portIndex = 0;
		}
		
		logMessage( "Trying next port... index: " + portIndex + ", name: " + Serial.list()[portIndex],
			    true );
		
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
	if ( message == null || message.isEmpty() || message.equals( OUTER_KEY ) )
	{
	    return;
	}

	JSONObject json = parseJSONObject( message );

	if ( json == null )
	{
	    return;
	}		

	// ********************************************************* //
	// ************* PLOT SETUP FROM CONFIG CODE *************** //
	// ********************************************************* //

	String tempCode = "";
	boolean config = false;
	if ( json.hasKey( "ng" ) && json.hasKey( "lu" ) )
	{
	    tempCode = Integer.toString( json.getInt( "ng" ) ) + Integer.toString( json.getInt( "lu" ) );
	    config = true;
	}
	
	// If config code has changed, need to go through setup again
	if ( config && !configCode.equals( tempCode ) )
	{
	    lastPortSwitch = millis(); // (likely on the right port, just need to reconfigure graph layout)
	    
	    // Check for size of full transmission against expected to flag bad transmission
	    numGraphs = json.getInt( "ng" );
	    
	    JSONArray jsonGraphs = json.getJSONArray( "g" );
	    
	    if ( jsonGraphs.size() != numGraphs )
	    {
		return;
	    }

	    configured = false;
	    String concatLabels = "";
	    
	    // Setup new layout
	    float[][] posGraphs = setupGraphPosition( numGraphs );
      
	    graphs = new ArrayList<Graph>();
      
	    // Iterate through the individual graph data blocks to get graph specific info
	    for ( int i = 0; i < numGraphs; i++ )
	    {
		JSONObject g = jsonGraphs.getJSONObject( i );
		
		String title = g.getString( "t" );
		boolean xvyTemp = g.getInt( "xvy" ) == 1;
		int maxPoints = g.getInt( "pd" );
		int numVars = g.getInt( "sz" );
		String[] labelsTemp = new String[numVars];
		int[] colorsTemp = new int[numVars];

		concatLabels += title;

		JSONArray l = g.getJSONArray( "l" );
		JSONArray c = g.getJSONArray( "c" );
		for ( int j = 0; j < numVars; j++ )
		{
		    labelsTemp[j] = l.getString( j );
		    colorsTemp[j] = COLORMAP.get( c.getString( j ) );
		    if ( colorsTemp[j] == 0 )
		    {
			logMessage( "Invalid color: " + c.getString( j ) + ", defaulting to green.", true );
			colorsTemp[j] = COLORMAP.get( "green" );
		    }
		    concatLabels += labelsTemp[j];
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
	    
	    // Set new config code
	    if ( concatLabels.equals( lastLabels ) ) // Only when we're sure on labels
	    {
		configCode = tempCode;
		lastConfig = millis();
		logMessage( "Configured " + graphs.size() + " graphs", false ); 
	    }
	    lastLabels = concatLabels;

	    logMessage( "Config code: " + configCode + ", Label config: " + concatLabels, true );
	}
	else
	{
	    // Matching a code means we have configured correctly
	    configured = true;
          
	    // *********************************************************** //
	    // ************ NORMAL PLOTTING FUNCTIONALITY **************** //
	    // *********************************************************** //
	    int tempTime = json.getInt( "t" );
      
	    JSONArray jsonGraphs = json.getJSONArray( "g" );

	    for ( int i = 0; i < numGraphs; i++ )
	    {
		JSONArray data = jsonGraphs.getJSONObject( i ).getJSONArray( "d" );

		double[] tempData = new double[ data.size() ];

		// Update graph objects with new data
		for ( int j = 0; j < data.size(); j++ )
		{
		    tempData[j] = data.getDouble( j );
		}
		graphs.get( i ).Update( tempData, tempTime );
	    }
	}
    }
    catch ( Exception e )
    {
	logMessage( "Exception in serialEvent: " + e.toString(), true );
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
    if ( index >= Serial.list().length )
    {
	return;
    }
    String portName = Serial.list()[portIndex];
    logMessage( "Attempting connect on port: " + portName, false );

    // Wrap Serial port connect in future to force timeout
    ExecutorService exec = Executors.newSingleThreadExecutor();
    Future<Serial> future = exec.submit( new ConnectWithTimeout( this, portName, BAUD_RATE ) );    

    try
    {
	// Close port if another is open
	if ( port != null && port.active() )
	{
	    port.stop();
	}
	
	// Do connect with timeout
	port = future.get( CONNECT_TIMEOUT, TimeUnit.MILLISECONDS );

	lastPortSwitch = millis(); // at end so that we try again immediately on invalid port
	logMessage( "Connected on " + portName + ". Listening for configuration...", false );
    }
    catch ( TimeoutException e )
    {
	future.cancel( true );
	logMessage( "Timed out.", true );
    }
    catch ( Exception e )
    {
	logMessage( "Exception on connect: " + e.toString(), true );	
    }

    exec.shutdownNow();
}

// Callable class to wrap Serial connect
class ConnectWithTimeout implements Callable<Serial>
{
    private final PApplet parent;
    private final String portName;
    private final int baudRate;

    public ConnectWithTimeout( PApplet parent, String portName, int baud )
    {
	this.parent = parent;
	this.portName = portName;
	this.baudRate = baud;
    }
  
    @Override
	public Serial call() throws Exception
    {
	return new Serial( this.parent, this.portName, baudRate );
    }     
}

// Logger helper
void logMessage( String message, boolean debugOnly )
{
    if ( DEBUG || !debugOnly )
    {
	String level = debugOnly ? "DEBUG" : "STATUS";
	println( "[Time: " + millis() + " ms]" + "[" + level + "] " + message );
    }
}


		 