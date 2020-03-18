#version 330

layout (location=0) in vec4 position;
layout (location=1) in vec3 in_color;
layout (location=2) in vec3 translation;
layout (location=3) in vec3 normal;
layout (location=4) in vec4 objTransC1;
layout (location=5) in vec4 objTransC2;
layout (location=6) in vec4 objTransC3;
layout (location=7) in vec4 objTransC4;
layout (location=8) in float selectionStatus; 
varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec4 glPosition0;
varying mat4 lighting_transform;

in int gl_InstanceID;

uniform mat4 transform;
uniform vec3 camera_position;
uniform float time;

out vec3 out_color;
out vec3 out_normal;
out vec3 FragPos;


mat4 rotationY = mat4(	cos(0.01*time),		0,		sin(0.01*time),	0,
			 				0,		1.0,			 0,	0,
					-sin(0.01*time),	0,		cos(0.01*time),	0,
							0, 		0,				0,	1);



float random (vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}
 void main() 
{
	mat4 x = mat4(objTransC1, objTransC2, objTransC3, objTransC4);
	gl_Position = (transform) * x * (vec4(position.xyz, 1)) ; 
	//rotationY *
	


	FragPos = (transform*vec4(position.xyz,1)).xyz;
	
	lighting_transform = transform;
	glPosition0 = (position);

		normal0 = normal;
		float r = 0.8 * sin(0.002*time);
		float g = 0.9 - 0.5*0.86*cos(0.0001*time);
	float b = 0.3;
	
		//if (gl_InstanceID == 0) b = 1;
			//else if (gl_InstanceID % 2 == 0) 1;
			//else g =  1;
		if (selectionStatus > 0.5 ) g = 1.0;
		else g = 0.0;
		out_color = vec3(r,g,b);
}