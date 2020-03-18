#version 330

layout (location=0) in vec4 position;
layout (location=1) in vec3 in_color;
layout (location=2) in vec2 texCoord;
layout (location=3) in vec3 normal;




varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec4 prox;
uniform mat4 transform;
uniform vec3 camera_position;

out vec3 out_color;
out vec3 out_normal;


 void main() 
		
	{

	gl_Position = transform*vec4( position.x,position.y,position.z,1 ); 

	prox = vec4(camera_position,1);
	out_color = in_color;
		normal0 = normal;
	position0 = vec3(position);
	}