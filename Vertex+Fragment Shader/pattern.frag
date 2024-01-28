#version 330 //compatibility

uniform float	uTime;		// "Time", from Animate( )

uniform float	uBlend = 0.5; 

	
in vec3		vColor;	
uniform bool 	uDisplayFrag = true;
in vec2  	vST;	

void
main( )
{
	if(uDisplayFrag) {
		vec3 myColor;
		
		if( vST.t < uTime || uTime < (vST.t - .3) ) {
			myColor = vec3(0., 0.2, 0.);
		}
		else {
			myColor = vec3(0.5, 0.5, 0.);
		}
		
		vec3 finalColor = myColor * (1 - uBlend) + vColor * (uBlend);
		
		gl_FragColor = vec4(finalColor, 1.);
	}
	else {
		gl_FragColor = vec4( 0. , 0.2 , 0. , 1.);
	}
}
