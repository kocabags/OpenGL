**************

Introduction:

 * The goal of this project is to create an animated pattern by using OpenGL vertex and fragment          shaders. The choice of pattern is up to you, but it cannot be a rectangle, because that's what the    un-imaginative Joe Graphics showed you how to do.
 
**************

Requirements:

 1. Draw a 3D object (your choice).
 
 2. Use a vertex shader to place a vertex-changing pattern on it. The pattern must be different for different parts of the object. Key off of the (s,t) coordinates or the (x,y,z) coordinates. Doing the exact same operation to all vertices doesn't count!
 
 3. Use a fragment shader to place a color-changing pattern on it. The pattern make not be a rectangle. The pattern must be different in different parts of the object. Key off of the (s,t) coordinates or the (x,y,z) coordinates. Doing the exact same operation to all fragments doesn't count!
 
 4. The choice of patterns is (almost) up to you (see above).
 
 5. The choice of colors is up to you.
 
 6. Perform some sort of animation (your choice) on the patterns. That is, make the vertex and fragment patterns change with time.
 
 7. You can key off of anything you would like. Typically that is XYZ in model coordinates, XYZ in world coordinates, or ST. Be aware that, in the GLUT library, only the teapot has ST coordinates. The OsuSphere( ) also does.
 
 8. You must have the ability to show both patterns animating at the same time, and the ability to have each pattern animating separately. One way to control this is with keyboard keys:
 
|  Keys  |                                Description                                       |
| ------ | -------------------------------------------------------------------------------- |
|  'b'   |    Animate both the vertex and fragment shader patterns together                 |
|  'f'   |    Freese both animations                                                        |
|  'F'   |    Animate just the fragment shader pattern, freezing the vertex shader pattern  |
|  'V'   |    Animate just the vertex shader pattern, freezing the fragment shader pattern  |  


