#pragma once
#include <glm/glm.hpp>
#include <vector>
#include<glew.h>
#include <glut.h>
#include <GL/GL.h>
#include "shader.h"
#define GLM_ENABLE_EXPERIMENTAL

#include "transform.h"
#include "display.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


class GUI
{
public:

	void genGenGUI()
	{

		positions.push_back(glm::vec4(1,1,1,1));

	}

	GUI(Camera * cam) : shader(new Shader("./res/cubeShader")), camera(cam), transform(Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)))
	{
		glGenBuffers(1, vbo);
		glGenBuffers(1, vbo2);
		glGenBuffers(1, ebo);
		glGenBuffers(1, vao);

	}


	void DrawGUI()
	{
		glBindBuffer(GL_ARRAY_BUFFER, *vao);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, *vbo2);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLshort), indices.data(), GL_STATIC_DRAW);

		shader->Bind();
		shader->Update(transform, *camera);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
	}



protected:
	GLuint* ebo = new GLuint;
	GLuint* vbo = new GLuint;
	GLuint* vbo2 = new GLuint;
	GLuint* vao = new GLuint;

	std::vector <glm::vec4> positions;
	std::vector <GLshort> indices;
	std::vector <glm::vec3> colors;
	std::vector <glm::vec3> normals;
	Camera * camera;
	Transform transform;
	Shader * shader;


};