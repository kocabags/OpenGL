#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>


// title of these windows:

const char *WINDOWTITLE = { "Project 6 - Sarp Kocabagli" };
const char *GLUITITLE   = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 2000 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };


// which projection:

enum Projections
{
	ORTHO,
	PERSP
};


// which button:

enum ButtonVals
{
	RESET,
	QUIT
};


// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };


// line width for the axes:

const GLfloat AXES_WIDTH   = { 3. };


// the color numbers:
// this order must match the radio button order

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char *)"Yellow",
	(char *)"Green",
	(char *)"Cyan",
	(char *)"Blue",
	(char *)"Magenta",
	(char *)"White",
	(char *)"Black"
};


// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};


// fog parameters:

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };


// non-constant global variables:
bool	Frozen;
bool	CPoints;
bool	CLines;
int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to use the z-buffer
GLuint	BoxList;				// object display list
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees


struct Point
{
        float x0, y0, z0;       // initial coordinates
        float x = x0;       // animated coordinates
		float y = y0;       // animated coordinates
		float z = z0;       // animated coordinates
};

struct Curve
{
        float r, g, b;
        Point point1;
		Point point2;
		Point point3;
		Point point4;
        int count;
};

#define NUMCURVES	7
#define NUMPOINTS	50
Curve Curves[NUMCURVES];		// if you are creating a pattern of curves
float Time;
#define MS_IN_THE_ANIMATION_CYCLE	600

// function prototypes:
void	RotateX( Point *p, float , float , float , float );
void	RotateY( Point *p, float , float , float , float );
void	RotateZ( Point *p, float , float , float , float );
void 	ControlPoints( );
void 	ControlLines( );
void 	BezierCurve ( int );
void 	AnimBezierCurve ( int );
void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	Axes( float );
void	HsvRgb( float[3], float [3] );

// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );


	// setup all the graphics stuff:

	InitGraphics( );


	// create the display structures that will not change:

	InitLists( );


	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );


	// setup all the user interface stuff:

	InitMenus( );


	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );


	// this is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutMainLoop( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:
	

	int ms = glutGet( GLUT_ELAPSED_TIME );	// milliseconds
	ms  %=  MS_IN_THE_ANIMATION_CYCLE;
	Time = (float)ms  /  (float)MS_IN_THE_ANIMATION_CYCLE;        // [ 0., 1. )

	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	if( DebugOn != 0 )
	{
		fprintf( stderr, "Display\n" );
	}


	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( DepthBufferOn != 0 )
		glEnable( GL_DEPTH_TEST );
	else
		glDisable( GL_DEPTH_TEST );


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH ) *2;
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT ) *2;
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -3., 3.,     -3., 3.,     0.1, 1000. );
	else
		gluPerspective( 90., 1.,	0.1, 1000. );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0., 0., 3.,     0., 0., 0.,     0., 1., 0. );


	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0., 1., 0. );
	glRotatef( (GLfloat)Xrot, 1., 0., 0. );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure normals get unitized:

	glEnable( GL_NORMALIZE );


	// draw the current object:

	//1st point of right side of the heart
	Curves[0].point1.x0 = 0;
	Curves[0].point1.y0 = 0;
	Curves[0].point1.z0 = 0;

	//2nd point of right side of the heart
	Curves[0].point2.x0 = 1.126;
	Curves[0].point2.y0 = 0.535;
	Curves[0].point2.z0 = 0;

	//3rd point of right side of the heart
	Curves[0].point3.x0 = 0.53;
	Curves[0].point3.y0 = 2.026;
	Curves[0].point3.z0 = 0;

	//4th point of right side of the heart
	Curves[0].point4.x0 = 0;
	Curves[0].point4.y0 = 1;
	Curves[0].point4.z0 = 0;

	BezierCurve(0);

	//1st point of left side of the heart
	Curves[1].point1.x0 = 0;
	Curves[1].point1.y0 = 0;
	Curves[1].point1.z0 = 0;

	//2nd point of left side of the heart
	Curves[1].point2.x0 = -1.126;
	Curves[1].point2.y0 = 0.535;
	Curves[1].point2.z0 = 0;

	//3rd point of left side of the heart
	Curves[1].point3.x0 = -0.53;
	Curves[1].point3.y0 = 2.026;
	Curves[1].point3.z0 = 0;

	//4th point of left side of the heart
	Curves[1].point4.x0 = 0;
	Curves[1].point4.y0 = 1;
	Curves[1].point4.z0 = 0;

	BezierCurve(1);

	//Leaves

	//1st point of right side of the leaf
	Curves[2].point1.x0 = 0;
	Curves[2].point1.y0 = 0;
	Curves[2].point1.z0 = 0;

	//2nd point of right side of the leaf
	Curves[2].point2.x0 = 1.5;
	Curves[2].point2.y0 = 0;
	Curves[2].point2.z0 = -0.9;

	//3rd point of right side of the leaf
	Curves[2].point3.x0 = 1.5;
	Curves[2].point3.y0 = 0;
	Curves[2].point3.z0 = 0.9;

	//4th point of right side of the leaf
	Curves[2].point4.x0 = 0;
	Curves[2].point4.y0 = 0;
	Curves[2].point4.z0 = 0;

	RotateY(&Curves[2].point2, Time * 360 , 0 , 0 , 0 );
	RotateY(&Curves[2].point3, Time * 360 , 0 , 0 , 0 );
	RotateY(&Curves[2].point4, Time * 360 , 0 , 0 , 0 );
	AnimBezierCurve(2);


	//1st point of left side of the leaf
	Curves[3].point1.x0 = 0;
	Curves[3].point1.y0 = 0;
	Curves[3].point1.z0 = 0;

	//2nd point of left side of the leaf
	Curves[3].point2.x0 = -1.5;
	Curves[3].point2.y0 = 0;
	Curves[3].point2.z0 = -0.9;

	//3rd point of left side of the leaf
	Curves[3].point3.x0 = -1.5;
	Curves[3].point3.y0 = 0;
	Curves[3].point3.z0 = 0.9;

	//4th point of left side of the leaf
	Curves[3].point4.x0 = 0;
	Curves[3].point4.y0 = 0;
	Curves[3].point4.z0 = 0;

	RotateY(&Curves[3].point2, Time * 360 , 0 , 0 , 0 );
	RotateY(&Curves[3].point3, Time * 360 , 0 , 0 , 0 );
	RotateY(&Curves[3].point4, Time * 360 , 0 , 0 , 0 );
	AnimBezierCurve(3);

	//bottom part of the flower 

	//1st point 
	Curves[4].point1.x0 = 0;
	Curves[4].point1.y0 = -2;
	Curves[4].point1.z0 = 0;

	//2nd point 
	Curves[4].point2.x0 = 0.5;
	Curves[4].point2.y0 = -1.5;
	Curves[4].point2.z0 = 0;

	//3rd point 
	Curves[4].point3.x0 = 0.3;
	Curves[4].point3.y0 = -0.5;
	Curves[4].point3.z0 = 0;

	//4th point 
	Curves[4].point4.x0 = 0;
	Curves[4].point4.y0 = 0;
	Curves[4].point4.z0 = 0;

	BezierCurve(4);


	//bottom leaves

	//1st point 
	Curves[5].point1.x0 = 0.3;
	Curves[5].point1.y0 = -1;
	Curves[5].point1.z0 = 0;

	//2nd point 
	Curves[5].point2.x0 = 0.7;
	Curves[5].point2.y0 = 0.4;
	Curves[5].point2.z0 = 0;

	//3rd point 
	Curves[5].point3.x0 = 1.0;
	Curves[5].point3.y0 = 0;
	Curves[5].point3.z0 = 0;

	//4th point 
	Curves[5].point4.x0 = 0.3;
	Curves[5].point4.y0 = -1;
	Curves[5].point4.z0 = 0;

	BezierCurve(5);

	//1st point 
	Curves[6].point1.x0 = 0.28;
	Curves[6].point1.y0 = -1.4;
	Curves[6].point1.z0 = 0;

	//2nd point 
	Curves[6].point2.x0 = -0.1;
	Curves[6].point2.y0 = 0.1;
	Curves[6].point2.z0 = 0;

	//3rd point 
	Curves[6].point3.x0 = -0.4;
	Curves[6].point3.y0 = 0;
	Curves[6].point3.z0 = 0;

	//4th point 
	Curves[6].point4.x0 = 0.28;
	Curves[6].point4.y0 = -1.4;
	Curves[6].point4.z0 = 0;

	BezierCurve( 6 );

	ControlPoints( );
	ControlLines( );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}

void BezierCurve ( int i ) {
	
	glLineWidth( 40. );
	glColor3f( (i+1)*Time/2, 0.8-Time/2, 0.1 );
	glBegin( GL_LINE_STRIP );

	for( int it = 0; it <= NUMPOINTS; it++ )
	{
		float t = (float)it / (float)NUMPOINTS;
		float omt = 1.f - t;
		float x = omt*omt*omt*Curves[i].point1.x0 + 3.f*t*omt*omt*Curves[i].point2.x0 + 3.f*t*t*omt*Curves[i].point3.x0 + t*t*t*Curves[i].point4.x0;
		float y = omt*omt*omt*Curves[i].point1.y0 + 3.f*t*omt*omt*Curves[i].point2.y0 + 3.f*t*t*omt*Curves[i].point3.y0 + t*t*t*Curves[i].point4.y0;
		float z = omt*omt*omt*Curves[i].point1.z0 + 3.f*t*omt*omt*Curves[i].point2.z0 + 3.f*t*t*omt*Curves[i].point3.z0 + t*t*t*Curves[i].point4.z0;
		glVertex3f( x, y, z );
	}	
	glEnd( );
}

void AnimBezierCurve ( int i ) {
	
	glLineWidth( 40. );
	glColor3f( (i+1)*Time, 0.8-Time, 0.1 );
	glBegin( GL_LINE_STRIP );

	for( int it = 0; it <= NUMPOINTS; it++ )
	{
		float t = (float)it / (float)NUMPOINTS;
		float omt = 1.f - t;
		float x = omt*omt*omt*Curves[i].point1.x + 3.f*t*omt*omt*Curves[i].point2.x + 3.f*t*t*omt*Curves[i].point3.x + t*t*t*Curves[i].point4.x;
		float y = omt*omt*omt*Curves[i].point1.y + 3.f*t*omt*omt*Curves[i].point2.y + 3.f*t*t*omt*Curves[i].point3.y + t*t*t*Curves[i].point4.y;
		float z = omt*omt*omt*Curves[i].point1.z + 3.f*t*omt*omt*Curves[i].point2.z + 3.f*t*t*omt*Curves[i].point3.z + t*t*t*Curves[i].point4.z;
		glVertex3f( x, y, z );
	}	
	glEnd( );
}

void ControlPoints ( ) {
	int i = 0;
	while ( NUMCURVES > i ) 
	{
		if ( CPoints == 1 )
		{
			glColor3f( 1. , 1. , 1. );
			glPointSize( 10 );
			glBegin( GL_POINTS );
			glVertex3f( Curves[i].point1.x0 , Curves[i].point1.y0 , Curves[i].point1.z0 );
			glVertex3f( Curves[i].point2.x0 , Curves[i].point2.y0 , Curves[i].point2.z0 );
			glVertex3f( Curves[i].point3.x0 , Curves[i].point3.y0 , Curves[i].point3.z0 );
			glVertex3f( Curves[i].point4.x0 , Curves[i].point4.y0 , Curves[i].point4.z0 );
			glEnd( );
		}
		i++;
	}
}

void ControlLines ( ) {
	int i = 0;
	while ( NUMCURVES > i ) 
	{
		if ( CLines == 1 )
		{
			glColor3f( 1. , 1. , 1. );
			glBegin( GL_LINE_STRIP );
			glVertex3f( Curves[i].point1.x0 , Curves[i].point1.y0 , Curves[i].point1.z0 );
			glVertex3f( Curves[i].point2.x0 , Curves[i].point2.y0 , Curves[i].point2.z0 );
			glVertex3f( Curves[i].point3.x0 , Curves[i].point3.y0 , Curves[i].point3.z0 );
			glVertex3f( Curves[i].point4.x0 , Curves[i].point4.y0 , Curves[i].point4.z0 );
			glEnd( );
		}
		i++;
	}
}



void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Colors",        colormenu);
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );
	glutIdleFunc( Animate );

	// init glew (a window must be open to do this):


}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );
}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'f':
		case 'F':
			Frozen = ! Frozen;
			if( Frozen )
				glutIdleFunc( NULL );
			else
				glutIdleFunc( Animate );
			break;
			
		case '1':
			CPoints = !CPoints;
			break;

		case '2':
			CLines = !CLines;
			break;

		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}


	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "MouseMotion: %d, %d\n", x, y );


	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}


	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 0;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Scale  = 1.0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	if( DebugOn != 0 )
		fprintf( stderr, "ReSize: %d, %d\n", width, height );

	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = {
		0.f, 1.f, 0.f, 1.f
	      };

static float xy[ ] = {
		-.5f, .5f, .5f, -.5f
	      };

static int xorder[ ] = {
		1, 2, -3, 4
		};

static float yx[ ] = {
		0.f, 0.f, -.5f, .5f
	      };

static float yy[ ] = {
		0.f, .6f, 1.f, 1.f
	      };

static int yorder[ ] = {
		1, 2, 3, -2, 4
		};

static float zx[ ] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
	      };

static float zy[ ] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
	      };

static int zorder[ ] = {
		1, 2, 3, 4, -5, 6
		};

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r, g, b;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
RotateX( Point *p, float deg, float xc, float yc, float zc )
{
        float rad = deg * (M_PI/180.f);         // radians
        float x = p->x0 - xc;
        float y = p->y0 - yc;
        float z = p->z0 - zc;

        float xp = x;
        float yp = y*cos(rad) - z*sin(rad);
        float zp = y*sin(rad) + z*cos(rad);

        p->x = xp + xc;
        p->y = yp + yc;
        p->z = zp + zc;
}

void
RotateY( Point *p, float deg, float xc, float yc, float zc )
{
        float rad = deg * (M_PI/180.f);         // radians
        float x = p->x0 - xc;
        float y = p->y0 - yc;
        float z = p->z0 - zc;

        float xp =  x*cos(rad) + z*sin(rad);
        float yp =  y;
        float zp = -x*sin(rad) + z*cos(rad);

        p->x = xp + xc;
        p->y = yp + yc;
        p->z = zp + zc;
}

void
RotateZ( Point *p, float deg, float xc, float yc, float zc )
{
        float rad = deg * (M_PI/180.f);         // radians
        float x = p->x0 - xc;
        float y = p->y0 - yc;
        float z = p->z0 - zc;

        float xp = x*cos(rad) - y*sin(rad);
        float yp = x*sin(rad) + y*cos(rad);
        float zp = z;

        p->x = xp + xc;
        p->y = yp + yc;
        p->z = zp + zc;
}
