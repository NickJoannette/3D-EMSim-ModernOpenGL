#version 330 core

in vec4 out_color[];

out vec4 ex_color;
out float scal;

void main()
{
    scal = 0.5;
	ex_color.x = out_color[0].x;


	ex_color.y = out_color[0].y;

	ex_color.z = out_color[0].z;

	ex_color.w = 1;


}