

class Graph
{
    // Config
    int posY;
    int posX;
    int height;
    int width;
    
    boolean xvy;
    int numVars;
    int maxPoints;
    String title;
    String[] labels;

    // Data
    int posX;
    double[][] data;
    int[] extremesCounter;
    double[] extremes; // {min_x, max_x, min_y, max_y}

    // Contructor
    public Graph( int posY, int posX, int height, int width,
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

    // Modifiers
    public void Reconfigure( int posY, int posX, int height, int width,
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
    
    public void Update( double[] newData )
    {}

    public void Plot()
    {}

    // Internal Helpers
    private void DrawDataXY()
    {}

    private void DrawDataTime()
    {}

    private void DrawTicks()
    {}
}
