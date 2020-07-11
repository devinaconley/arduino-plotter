/*
  ===========================================================================================
  This Graph class handles an individual graph being displayed on the Arduino Plotter 
  listener.
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


import processing.core.PApplet;

class Graph
{
    // Reference to PApplet drawing canvas
    PApplet parent;
    
    // Config
    float posY;
    float posX;
    float height;
    float width;
    
    boolean xvy; // ( alternative is vs time )
    int numVars;
    int maxPoints;
    String title;
    String[] labels;
    int[] colors; // TODO: Configurable colors

    // Data
    int index;
    double[][][] data;
    int[] extremesCounter;
    double[] extremes; // {min_x, max_x, min_y, max_y}
    int currPoints;

    // Contructor
    public Graph( PApplet parent, float posY, float posX, float height, float width,
		  boolean xvy, int numVars, int maxPoints,
		  String title, String[] labels, int[] colors )
    {
	this.parent = parent;
	this.posY = posY;
	this.posX = posX;
	this.height = height;
	this.width = width;
	this.xvy = xvy;
	this.numVars = numVars;
	this.maxPoints = maxPoints;
	this.title = title;
	this.labels = labels;
	this.colors = colors;
	
	// this.parent.println( "Constructed new graph: ", this.title, " at ", this.posY, " ", this.posX );
	
	// Initialize
	this.index = 0;
	this.data = new double[maxPoints][numVars][2];
	this.extremesCounter = new int[4];
	this.extremes = new double[4];
	this.currPoints = 0;
    }

    // Modifiers
    public void Reconfigure( float posY, float posX, float height, float width,
			     boolean xvy, int numVars, int maxPoints,
			     String title, String[] labels )
    {
	this.posY = posY;
	this.posX = posX;
	this.height = height;
	this.width = width;
	this.xvy = xvy;
	this.numVars = numVars;
	this.maxPoints = maxPoints;
	this.title = title;
	this.labels = labels;	
    }
    
    public void Reconfigure( float posY, float posX, float height, float width )
    {
	this.posY = posY;
	this.posX = posX;
	this.height = height;
	this.width = width;
    }
    
    public void Update( double[] newData, int time )
    {
	// Store data
	if ( this.xvy )
	{
	    // Validate
	    if ( newData.length != 2 )
	    {
		//this.parent.println( "Invalid data passed to X v. Y graph." );
		return;
	    }
	    
	    this.data[this.index][0][0] = newData[0];
	    this.data[this.index][0][1] = newData[1];
	}
	else
	{
	    // Validate
	    if ( newData.length != this.numVars )
	    {
		//this.parent.println( "Invalid data passed to time graph." );
		return;
	    }

	    for ( int i = 0; i < this.numVars; i++ )
	    {
		this.data[this.index][i][0] = time;
		this.data[this.index][i][1] = newData[i];
	    }
	}

	// Counter for num points defined
	if ( this.currPoints < this.maxPoints )
	{
	    this.currPoints++;
	}
	
	// Check extremes
	this.CheckExtremes();

	// Advance index position and rollback if needed
	this.index++;
	if ( this.index >= this.maxPoints )
	{
	    this.index = 0;
	}
    }

    public void Plot()
    {
	// Plot Background
	this.parent.fill( PLOT_COL );
	this.parent.stroke( PLOT_COL );
	this.parent.strokeWeight( (this.width + this.height) / 2.0f * PT_SZ );
	this.parent.rect( this.posX, this.posY, this.width, this.height );

	// Title
	this.parent.textSize( (int)( (this.width + this.height) / 2.0f * TITLE_SZ ) );
	this.parent.fill( 255 );
	this.parent.textAlign( this.parent.CENTER, this.parent.TOP );
	this.parent.text( this.title, this.posX + this.width / 2, this.posY + 10 );

	
	// Calculations for offset and scaling of graph ( vs. time )
	double xScale = this.width / ( this.extremes[1] - this.extremes[0] );
	double xOffset = xScale * this.extremes[0];
	double yScale = AXIS_COV * this.height / ( this.extremes[3] - this.extremes[2] );
	double yOffset = yScale * this.extremes[3] + 0.5 * ( 1.0 - AXIS_COV ) * this.height;

	// Modify scaling and offset
	if ( this.xvy )
	{
	    xScale *= AXIS_COV;
	    xOffset = xScale * this.extremes[0] - 0.5 * ( 1.0 - AXIS_COV ) * this.width;
	}

	
        // Do actual data plotting
	for ( int i = 0; i < this.numVars; i++ )
	{
	    this.parent.stroke( this.colors[i] );
	    for ( int j = 0; j < this.currPoints; j++ )
	    {
		this.parent.point( (float)(this.posX + (this.data[j][i][0]*xScale - xOffset)),
				   (float)(this.posY + yOffset - data[j][i][1]*yScale) );
	    }
	}	
	
	// X vs Y and vs Time specific stuff
	if ( this.xvy )
	{
	    this.DrawXYStuff();	    
	}
	else
	{
	    this.DrawTimeStuff();
	}

	// Draw Ticks
	this.DrawTicks( xScale, xOffset, yScale, yOffset );
	
    }

    // Private Helpers
    
    private void DrawTimeStuff()
    {
	int labelSz = (int) ( (this.width + this.height) / 2.0f * LABEL_SZ );
	
	// Setup legend start
	float textPos = this.posY + labelSz;
	this.parent.textAlign( this.parent.RIGHT, this.parent.TOP );
	this.parent.textSize( labelSz );

	// Draw each legend entry
	for ( int i = 0; i < this.numVars; i++ )
	{
	    this.parent.fill( this.colors[i] );
	    this.parent.text( this.labels[i], this.posX + this.width - 10, textPos);
	    textPos += ( labelSz + labelSz/4 );
	    this.parent.stroke( this.colors[i] );
	}

    }

    private void DrawXYStuff()
    {
	// X and Y labels
	this.parent.textSize( (int)( (this.width + this.height) / 2.0f * LABEL_SZ) );
	this.parent.textAlign( this.parent.LEFT, this.parent.TOP );
	this.parent.text( this.labels[1], this.posX + 10, this.posY + 10);

	this.parent.textAlign( this.parent.RIGHT, this.parent.BOTTOM );
	this.parent.text( this.labels[0], this.posX + this.width - 10, this.posY + this.height - 3.5f*TICK_LEN);
    }


    private void DrawTicks( double xScale, double xOffset, double yScale, double yOffset )
    {
	// Label graph with numbered tick marks
	this.parent.stroke( 255 );
	this.parent.fill( 255 );
	this.parent.textSize( (int)( ( this.width + this.height ) / 2.0f * NUM_SZ )  );
	this.parent.textAlign( this.parent.LEFT, this.parent.CENTER );

	// Draw ticks along y-axis
	float tempX = this.posX - TICK_LEN / 2;
	float tickOffset = 0.5f * ( 1.0f - AXIS_COV ) * this.height; 
	float tickInterval = AXIS_COV * this.height / (NUM_TICKS - 1);
	for ( float tempY = this.posY + tickOffset; tempY <= this.posY + this.height - tickOffset;
	     tempY += tickInterval )
	{
	    float val = (float) ( ( ( yOffset + this.posY ) - (double)tempY ) / yScale );
	    String fmt = GetNumberFormat( val );
	    this.parent.line( tempX, tempY, tempX + TICK_LEN, tempY );
	    this.parent.text( String.format( fmt, val ), tempX + TICK_LEN + 5, tempY );
	}

	// x-axis
	this.parent.textAlign( this.parent.CENTER, this.parent.BOTTOM );
	float tempY = this.posY + this.height - TICK_LEN / 2;
	tickOffset = 0.5f * ( 1.0f - AXIS_COV ) * this.width; 
	tickInterval = AXIS_COV * this.width / (NUM_TICKS - 1);
	for ( tempX = this.posX + tickOffset; tempX <= this.posX + this.width - tickOffset;
	     tempX += tickInterval )
	{
	    float val = (float) ( ( (double)tempX + xOffset - this.posX ) / xScale ); 
	    this.parent.line( tempX, tempY, tempX, tempY + TICK_LEN );
	    if ( this.xvy )
	    {
		String fmt = GetNumberFormat( val );
		this.parent.text( String.format( fmt, val ), tempX, tempY - 5 );
	    }
	    else
	    {
		this.parent.text( String.format( "%d", (int)val ), tempX, tempY - 5 );		
	    }
	}	
	
    }

    private String GetNumberFormat( float value )
    {      
	int n = SIG_DIGITS;
	int d = 1;	
	while ( n > 0 && Math.round( Math.abs( value / d ) ) > 0 )
	{
	    n--;
	    d *= 10;
	}

	String fmt = "%" + Integer.toString( 1 + SIG_DIGITS - n ) + "." + Integer.toString( n );
	if ( ( Math.abs( value ) > 1000 || Math.abs( value ) < 0.001 ) && value != 0 )
	{
	    fmt += "e";
	}
	else
	{
	    fmt += "f";
	}
	return fmt;
    }

    private void CheckExtremes()
    {
	// Check new values
	this.CompareToExtremes( this.index );
	
	// Time extremes
	if ( ! this.xvy )
	{
	    // Get index of oldest data point
	    int oldest = this.index + 1;
	    if ( oldest >= this.currPoints )
	    {
		oldest = 0;
	    }
	    
	    if ( this.currPoints < this.maxPoints )
	    {
		// Estimate lower extreme
		this.extremes[0] = this.data[this.index][0][0]
		    - ( this.data[this.index][0][0] - this.data[oldest][0][0] )
		    * ( (double)this.maxPoints / (double)this.currPoints );
	    }
	    else
	    {
		// Normally just take oldest
		this.extremes[0] = this.data[oldest][0][0];
	    }
	    this.extremesCounter[0] = 0;
	    this.extremes[1] = this.data[this.index][0][0];
	    this.extremesCounter[1] = 0;
	}
	
        // Check for extremes going out of scope
	boolean recalc = false;
	for ( int i = 0; i < 4; i++ )
	{
	    this.extremesCounter[i]++;
	    recalc |= this.extremesCounter[i] > this.maxPoints;
	}
	
	if ( ! recalc )
	{
	    return;
	}

	// this.parent.println("Recalculating extremes...");
	
	// Full re-calculation for new extremes
	if ( this.xvy )
	{
	    this.extremes[0] = this.data[0][0][0]; // (x-min)
	    this.extremesCounter[0] = 0;
	    this.extremes[1] = this.data[0][0][0]; // (x-max)
	    this.extremesCounter[1] = 0;
	}
	this.extremes[2] = this.data[0][0][1]; // (y-min)
	this.extremesCounter[2] = 0;
	this.extremes[3] = this.data[0][0][1]; // (y-max)
	this.extremesCounter[3] = 0;
	
	for ( int i = 0; i < this.currPoints; i++ )
	{
	    this.CompareToExtremes( i );
	}
    }

    private void CompareToExtremes( int i ) // i : dataIndex
    {		
	for ( int j = 0; j < this.numVars; j++ )
	{			
	    // Y max/min
	    if ( this.data[i][j][1] < this.extremes[2] ) // (min)
	    {
		this.extremes[2] = this.data[i][j][1];
		this.extremesCounter[2] = 0;
	    }
	    else if ( this.data[i][j][1] > this.extremes[3] ) // (max)
	    {
		this.extremes[3] = this.data[i][j][1];
		this.extremesCounter[3] = 0;
	    }
	    // X max/min
	    if ( this.xvy )
	    {
		if ( this.data[i][j][0] < this.extremes[0] ) // (min)
		{
		    this.extremes[0] = this.data[i][j][0];
		    this.extremesCounter[0] = 0;
		}
		else if ( this.data[i][j][0] > this.extremes[1] ) // (max)
		{
		    this.extremes[1] = this.data[i][j][0];
		    this.extremesCounter[1] = 0;
		}
	    }
	}
    }    

    // Constants
    private static final float AXIS_COV = 0.75f;
    private static final int PLOT_COL = 115;
    private static final float LABEL_SZ = 0.025f;
    private static final float TITLE_SZ = 0.03f;
    private static final float NUM_SZ = 0.02f;
    private static final int TICK_LEN = 6;
    private static final int NUM_TICKS = 5;
    private static final float PT_SZ = 0.0025f;
    private static final int SIG_DIGITS = 3;
}
