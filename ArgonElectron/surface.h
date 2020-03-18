#ifndef SURFACE_H
#define SURFACE_H
#include <glm/glm.hpp>
#include <vector>
#include<glew.h>
#include <GL/GL.h>
#include "shader.h"
#include "transform.h"
#define GLM_ENABLE_EXPERIMENTAL

#include "display.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

class Surface
{
public:

	Surface(unsigned int w, unsigned int l, bool f, Camera * c, unsigned int scale);
	void Draw();
	~Surface();

private:

	// Variables for rendering
	GLuint* ebo = new GLuint;
	GLuint* vbo = new GLuint;
	GLuint* vbo2 = new GLuint;
	GLuint* vao = new GLuint;

	std::vector <glm::vec4> positions;
	std::vector <GLshort> indices;
	std::vector <glm::vec3> normals;
	std::vector <glm::vec3> colors;

	Camera * camera;
	Transform transform;
	Shader * shader;

	// Geometric variables
	
	unsigned int width, length;
	bool flat;
};

#endif