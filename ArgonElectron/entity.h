#pragma once
#include <glm\glm.hpp>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL

#include<glew.h>
#include <GL/GL.h>
#include "shader.h"
#include "transform.h"
#include "display.h"
#include <chrono>
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


enum {
	NEUTRAL,
	POSITIVE,
	NEGATIVE
};

class cubicEntity
{
public:

	struct geometry
	{
		glm::vec4 positions[8] = {
			glm::vec4(-1, 1, 1, 1),
			glm::vec4(1, 1, 1, 1),
			glm::vec4(-1, -1, 1, 1),
			glm::vec4(1, -1, 1, 1),
			glm::vec4(-1, 1, -1, 1),
			glm::vec4(1,1,-1,1),
			glm::vec4(-1,-1,-1,1),
			glm::vec4(1,-1,-1,1)
		};

		glm::vec3 normals[36] = {
			glm::vec3(0, 0, -1),glm::vec3(0, 0, -1),glm::vec3(0, 0, -1),glm::vec3(0, 0, -1),glm::vec3(0, 0, -1),glm::vec3(0, 0, -1), // Front normals
			glm::vec3(1, 0, 0),glm::vec3(1, 0, 0),glm::vec3(1, 0, 0),glm::vec3(1, 0, 0),glm::vec3(1, 0, 0),glm::vec3(1, 0, 0), // Right normals
			glm::vec3(0, 0, 1),glm::vec3(0, 0, 1),glm::vec3(0, 0, 1),glm::vec3(0, 0, 1),glm::vec3(0, 0, 1),glm::vec3(0, 0, 1), // Back normals
			glm::vec3(-1, 0, 0),glm::vec3(-1, 0, 0),glm::vec3(-1, 0, 0),glm::vec3(-1, 0, 0),glm::vec3(-1, 0, 0),glm::vec3(-1, 0, 0), // Left normals
			glm::vec3(0, 1, 0),glm::vec3(0, 1, 0),glm::vec3(0, 1, 0),glm::vec3(0, 1, 0),glm::vec3(0, 1, 0),glm::vec3(0, 1, 0), // Top normals
			glm::vec3(0, -1, 0),glm::vec3(0, -1, 0),glm::vec3(0, -1, 0),glm::vec3(0, -1, 0),glm::vec3(0, -1, 0),glm::vec3(0, -1, 0) // Bottom normals
		};

		GLshort indices[36] = {
			2,3,0, 0,3,1,// Front face
			3,7,1, 1,7,5,// Right face
			7,6,5, 5,6,4,// Back face
			6,2,4, 4,2,0,// Left face
			0,1,4, 4,1,5,// Top face
			6,7,2, 2,7,3// Bottom face
		};


		glm::vec3 colors[8];
	
	};

	cubicEntity(float sz, float chg, float ms, glm::vec3 pos, glm::vec3 vel, glm::vec3 netForce, Camera * cam): charge(chg),size(sz),mass(ms),velocity(vel),position(pos),
		shader( new Shader("./shaders/cubeShader")), camera(cam), sign(chg > 0 ? POSITIVE : NEGATIVE)
	{

		Transform  trans(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1,1,1));
		transform = trans;
		transform.SetPos(pos);
		transform.SetScale(glm::vec3(sz,sz,sz));
		if (sign == POSITIVE)
			for (int i = 0; i < 8; ++i)
				geometry.colors[i] = glm::vec3(sinf(pos.y), sinf(pos.x), 0);

		else if (sign == NEGATIVE)
				for (int i = 0; i < 8; ++i)
					geometry.colors[i] = glm::vec3(1, 0.1, 0.1);

		glGenBuffers(1, vbo);
		glGenBuffers(1, vbo2);
		glGenBuffers(1, ebo);
		glGenBuffers(1, vao);


	} 


	void Draw(bool frozen)
	{
		// Statement below is used to make all negative charges stationary (relatively solid parts of a larger structure)
		//if (sign == POSITIVE) if (!frozen) *transform.GetPos() += 0.0005f*velocity;
		if (!frozen && sign == POSITIVE) *transform.GetPos() += 0.00001f*velocity;

		/*
		Are stable structures formed by physical relationships betwixt negatively and positively charged basic particles which have resulted in
		motive stasis, or relatively static ones as possibly formed within the bounds of the electromagnetic
		conditions imposed by external bodies?
		*/

		/*glBindBuffer(GL_ARRAY_BUFFER, *vao);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec4), geometry.positions, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, *vbo2);
		glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(glm::vec3), geometry.colors, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glBindBuffer(GL_ARRAY_BUFFER, *vbo);
		glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), geometry.normals, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLshort), geometry.indices, GL_DYNAMIC_DRAW);


		shader->Bind();
		shader->Update(transform, *camera);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
		*/

	}

	// ACCESSORS AND MUTATORS
	glm::vec3 * NetForce(){return & netForce;}
	std::vector <glm::vec3> * ForcesActing(){return &forcesActing;}
	glm::vec3 * Position(){return transform.GetPos();}
	glm::vec3 * Velocity(){return &velocity;}
	glm::vec3 * Acceleration(){return &acceleration;}
	Transform * trnsfrm(){return &transform;}
	geometry * Geometry() { return &geometry; }
	static unsigned int vertexBufferSize() { return 8 * sizeof(glm::vec4); }
	static unsigned int indexBufferSize() { return 36 * sizeof(GLshort); }
	static unsigned int normalBufferSize() { return 36 * sizeof(glm::vec3); }
	static unsigned int colorBufferSize() { return 8 * sizeof(glm::vec3); }

	inline float  getCharge(){return charge;}
	inline float  getSign() { return sign; }
	/////////////////////////////
	float size;
	float charge;
	float mass;
	glm::vec3 mg;

private:

	// Render related variables
	geometry geometry;
	std::vector <glm::vec4> positions;
	std::vector <GLshort> indices;
	std::vector <glm::vec3> normals;
	std::vector <glm::vec3> colors;
	
	GLuint* ebo = new GLuint;
	GLuint* vbo = new GLuint;
	GLuint* vbo2 = new GLuint;
	GLuint* vao = new GLuint;

	Camera * camera;
	Transform transform;
	Shader * shader;

	
	// Physic handling related variables
	
	std::chrono::steady_clock::time_point t_start = std::chrono::high_resolution_clock::now();
	
	std::vector <glm::vec3> forcesActing;
	glm::vec3 netForce;
	glm::vec3 center;
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	unsigned int sign;
	bool isFixedCharge = false;

	

};
