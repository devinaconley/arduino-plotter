import processing.core.PApplet;

// CONST



class Graph
{
    // Reference to PApplet drawing canvas
    PApplet parent;
    
    // Config
    float posY;
    float posX;
    float height;
    float width;
    
    boolean xvy;
    int numVars;
    int maxPoints;
    String title;
    String[] labels;

    // Data
    int index;
    double[][] data;
    int[] extremesCounter;
    double[] extremes; // {min_x, max_x, min_y, max_y}

    // Contructor
    public Graph( PApplet parent, float posY, float posX, float height, float width,
		  boolean xvy, int numVars, int maxPoints,
		  String title, String[] labels )
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

	this.parent.println( "Constructed new graph: ", this.title, this.posY, " ", this.posX);

	// Initialize
	index = 0;
	data = new double[maxPoints][numVars];
	extremesCounter = new int[4];
	extremes = new double[4];
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
    
    public void Reconfigure( int posY, int posX, int height, int width )
    {
	this.posY = posY;
	this.posX = posX;
	this.height = height;
	this.width = width;
    }
    
    public void Update( double[] newData )
    {}

    public void Plot()
    {
	// Plot Background
	this.parent.fill( PLOT_COL );
	this.parent.stroke( 255 );
	this.parent.rect( this.posX, this.posY, this.width, this.height );

	// Title
	this.parent.textSize( TITLE_SZ );
	this.parent.fill( 255 );
	this.parent.textAlign( this.parent.CENTER, this.parent.TOP );
	this.parent.text( this.title, this.posX + this.height/2, this.posY + TITLE_SZ);

	
	// Calculations for offset and scaling of graph ( vs. time )
	double xScale = this.width / ( this.extremes[1] - this.extremes[0] );
	double xOffset = xScale * this.extremes[0];
	double yScale = AXIS_COV * this.height / ( this.extremes[3] - this.extremes[2] );
	double yOffset = yScale * this.extremes[3] + 0.5 * ( 1.0 - AXIS_COV ) * this.height;

	// Draw
	if ( this.xvy )
	{
	    this.DrawDataXY( xScale, yScale, xOffset, yOffset );
	}
	else
	{
	    this.DrawDataTime( xScale, yScale, xOffset, yOffset );
	}
    }

    // Internal Helpers    
    private void DrawDataTime( double xScale, double yScale, double xOffset, double yOffset )
    {}

    private void DrawDataXY( double xScale, double yScale, double xOffset, double yOffset )
    {}


    private void DrawTicks()
    {}

    // Constants
    private static final float AXIS_COV = 0.75f;
    private static final int PLOT_COL = 115;
    private static final int LABEL_SZ = 14;
    private static final int TITLE_SZ = 16;
    private static final int NUM_SZ = 10;
    private static final int TICK_LEN = 6;
    private static final int NUM_TICKS = 5;
    private static final float PT_SZ = 1.5f;
}
