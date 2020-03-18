#version 330 

in vec3 out_color;
in vec3 out_normal;
in vec3 FragPos;
varying vec4 prox;
varying mat4 lighting_transform;

uniform sampler2D diffuse;

varying vec2 texCoord0;
varying vec3 normal0;
varying vec3 position0;
varying vec4 glPosition0;




void main() {
	vec3 color = out_color;
//	gl_FragColor = vec4(out_color,1);
	
	vec3 lightdirection = vec3(0,1,0.05);
	vec3 ambient = vec3(0.1,0.1,0.1);
	float cosineincidence = (dot(lightdirection,normal0)/(length(lightdirection)*length(normal0)));
	
	vec3 lightcolor = vec3(0.5,1.0,1.0);
	
	vec3 diffuse = cosineincidence * lightcolor;
	
	vec3 result = ((ambient + diffuse) * color) ;
	gl_FragColor = vec4(result,1);
 }