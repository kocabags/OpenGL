***********************
Introduction

This project is to animate a helicopter and look at it in two different 3D views.

***********************

Requirements:

 1. Draw a helicopter.
 
 2. The helicopter's 2 blades must be scaled properly.
  * The helicopter's 2 blades must be oriented properly.
  * The helicopter's 2 blades must be rotating properly.
 
 3. Allow two views: an "Outside" view of the entire scene and an "Inside" view from the helicopter cockpit. Toggle between them with a pop-up menu. (You can also use keyboard hits, but in-addition-to, not instead-of.) For each view, use a different call to gluLookAt( ) to position the eye. A good location for your Inside Eye is (-0.4, 1.8, -4.9).
 
 4. Keep the same rotate and scale features as we've used before, but only in the Outside View.
 
 5. Use gluPerspective( ), not glOrtho( ).
 
 6. Put some sort of 3D scene for your Inside Eye to see, looking outside the helicopter. One or more GLUT wireframe objects might work well. Or, whatever you did in the first project. Or some sort of colored grid. Or, all of these things.
 
 7. Use the graphics programming strategy where the Display( ) function looks at a collection of global variables and draws the scene correctly. The other parts of the program simply set the global variables and post a redisplay.
 
 8. The code for creating the helicopter geometry is shown below in the Geometry section. The helicopter body extends along the Z axis. The heicopter cockpit points in -Z.
 
 9. There must be two blades drawn on the helicopter body: a large blade on the roof, oriented in the X-Z plane, and a small one on the tail, oriented in the Y-Z plane. Create a single blade display list and then transform and instance it twice to get the two blades in the scene. The code for creating the single blade geometry is shown below in the Geometry section. The single blade is in the X-Y plane, centered at (0.,0.,0.), with a radius of 1.0.
 
 10. The top helicopter blade is to have a radius of 5. and be attached at: (0.,2.9,-2.).
 
 11. The rear helicopter blade is to have a radius of 1.5 and be attached at: (.5,2.5,9.).
 
 12. Both blades need to rotate. The rotation rate is up to you, but the smaller one must rotate 3X as fast as the larger one. That is, for whatever amount you advance the rotation the large blade's rotation angle, advance the small blade's rotation angle 3X that amount.
 
 13. Pay close attention to the overall transformation sequence. You can re-use some of the transformations that you have already created by using glPushMatrix( ) and glPopMatrix( ).
 
 14. Parameterize your scene as much as you can with #define's or const's, It makes it easier to make changes later.
 
***********************

Supplying the Geometry:

 * Get the fileLinks to an external site. called heli.550. Move it into your project workspace directory and then #include it in your C/C++ code just after your list of global variables.
 * The heli.550 file has the structure definitions in it, so you won't need to define them yourself.
 * If you want to draw a wireframe helicopter, use the following code when you create your helicopter display list:
``` 
        int i;
        struct edge *ep;
        struct point *p0, *p1;
        
        glPushMatrix( );
        glTranslatef( 0., -1., 0. );
        glRotatef(  97.,   0., 1., 0. );
        glRotatef( -15.,   0., 0., 1. );
        glBegin( GL_LINES );
        for( i=0, ep = Heliedges; i < Helinedges; i++, ep++ )
        {
		p0 = &Helipoints[ ep->p0 ];
		p1 = &Helipoints[ ep->p1 ];
		glVertex3f( p0->x, p0->y, p0->z );
		glVertex3f( p1->x, p1->y, p1->z );
        }
        glEnd( );
        glPopMatrix( );

```
 * If you want to draw a polygon helicopter, use the following code when you create your helicopter display list:
 
 ```
        int i;
        struct point *p0, *p1, *p2;
        struct tri *tp;
        float p01[3], p02[3], n[3];

        glPushMatrix( );
	glTranslatef( 0., -1., 0. );
	glRotatef(  97.,   0., 1., 0. );
	glRotatef( -15.,   0., 0., 1. );
	glBegin( GL_TRIANGLES );
		for( i=0, tp = Helitris; i < Helintris; i++, tp++ )
		{
			p0 = &Helipoints[ tp->p0 ];
			p1 = &Helipoints[ tp->p1 ];
			p2 = &Helipoints[ tp->p2 ];

			// fake "lighting" from above:

			p01[0] = p1->x - p0->x;
			p01[1] = p1->y - p0->y;
			p01[2] = p1->z - p0->z;
			p02[0] = p2->x - p0->x;
			p02[1] = p2->y - p0->y;
			p02[2] = p2->z - p0->z;
			Cross( p01, p02, n );
			Unit( n, n );
			n[1] = fabs( n[1] );
			n[1] += .25;
			if( n[1] > 1. )
				n[1] = 1.;
			glColor3f( 0., n[1], 0. );

			glVertex3f( p0->x, p0->y, p0->z );
			glVertex3f( p1->x, p1->y, p1->z );
			glVertex3f( p2->x, p2->y, p2->z );
		}
		glEnd( );
		glPopMatrix( );
```
***********************
