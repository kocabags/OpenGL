***************

Introduction: 

 * The goal of this project is to create a 3D animated scene that demonstrates OpenGL lighting.

***************

Requirements:

 1. Create a 3D scene consisting of at least 3 objects and at least 3 light sources:
 
*****************  Requirements Table  *****************

|    Category     | Minimum number | Minimum number |
| --------------- | -------------- | -------------- |
| Light motion    |  2 stationary  |  1 moving      |
| Light types     |  1 point       |  1 spotlight   |
| Light colors    |  1 white       |  1 color       |
| Obj material    |	1 shiny	       |  1 dull        |
| Obj ShadeModel  |	1 GL_FLAT	     |  1 GL_SMOOTH   |
| Obj motion      |	1 stationary   |	 1 moving      |
                                  

 2. Notice that there only need to be 3 light sources total. Clearly you can be double-dipping with the motion, types, and colors.

 3. Be sure to spread things out enough that we can see the effect of the 3 lights.

 4. Use RGB color for each object's GL_FRONT.
    Use a shade of gray for each object's GL_BACK.
 
 5. Put small spheres at the light source locations so that you and I can see where they are. Make each sphere the same color as the light source it is representing. Don't use lighting on the light-source spheres. Just make them glColor3f blobs.

 6. Feel free to use the GLUT solids for your objects. All have normals included.

 7. Or, you can design your own 3D objects. Just remember that they will need, at the least, per-face surface normals. If they are to be smooth-shaded, they will need per-vertex surface normals.

 8. Remember that light positions automatically get transformed by the GL_MODELVIEW transformation as it exists at the moment you specify the light position. You can't prevent this from happening. You can only control the GL_MODELVIEW matrix to be what you need it to be at the time.

 9. Enable the following keys:
            
|  Keys  |           Description              |
| ------ | ---------------------------------- |
|  '0'   |    Toggle light #0 off/on          |
|  '1'   |    Toggle light #1 off/on          |
|  '2'   |    Toggle light #2 off/on          |
|  'f'   |    Freese/un-freeze the animation  |   
              
 10. Even if you weren't required to turn lights off and on, you would want to anyway. This really helps debug lighting programs.

 11. You can do anything with the attenuation that you'd like.

 12. Feel free to use the "Shortcut" functions from the Lighting notes.

 13. Even though you have lots of flexibility about how to create the scene, you must make it obvious to us that it is handling the lighting correctly!



