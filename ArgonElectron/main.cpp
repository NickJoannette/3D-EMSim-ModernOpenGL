#include "stdafx.h"
#include <GLEW/glew.h>
#include <glut.h>
#include <glfw3.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <String>
#include <iostream>
#include "cubic_entity.h"
#include "texture.h"
#include "camera.h"
#include "transform.h"
#include "shader.h"
#include "display.h"
#include "mesh.h"
#include "entity.h"
#include "electron.h"
#include "physicshandler.h"
#include "surface.h"
#include "freetypehelper.h"
#undef main


extern std::vector <cubicEntity> cubic_entities;
Camera camera(glm::vec3(0, 0, 0), 70.0, (float)1440 / (float)900, 1.0f, 1000.0f);
Display display(1440, 900, camera);
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

GLuint masterEntityVertexBufferID;
GLuint masterEntityColorBufferID;
GLuint masterEntityNormalBufferID;
GLuint masterEntityIndexBufferID;
GLuint masterEntityTranslationBufferID;
GLuint masterEntityTransformBufferID;
GLuint masterEntitySelectionStatusBufferID;

Shader electronShader("./shaders/cubeShader");
Mesh testObj("./res/obj.obj");

freetypehelper::font_data our_font;

const int sphereCount = 64;
glm::vec3 translations[sphereCount];
Transform sphereTransforms[sphereCount];
glm::mat4 sphereTransformMatrices[sphereCount];
GLfloat sphereSelectionStatuses[sphereCount];
Electron electrons[sphereCount];

extern GLfloat * sS = &sphereSelectionStatuses[0];
extern Electron * eE = &electrons[0];
int indiceCount = 0;

void bufferSetup() {
	our_font.init("./fonts/vespasian.ttf", 72);


	/*

	-------------------------------------------
					CREATE SPHERE
--------------------------------------------------------

	*/
	// clear memory of prev arrays
	/*

	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLshort> indices;

	for (int i = 0; i < im.indices.size(); i++) {
		indices.push_back((GLshort)im.indices[i]);
	}
	for (int i = 0; i < im.positions.size(); i++) {
		vertices.push_back(glm::vec4(0.01f*im.positions[i], 1.0));
	}
	for (int i = 0; i < im.normals.size(); i++) {
		normals.push_back(im.normals[i]);
	}
	indiceCount = indices.size(); */
	
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;

	int sectorCount = 10;
	int stackCount = 6;
	float radius = 1;
	float PI = 3.14159;

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // vertex normal
	float s, t;                                     // vertex texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; ++i)
	{
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

		// add (sectorCount+1) vertices per stack
		// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j)
		{
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			// vertex position (x, y, z)
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			vertices.push_back(glm::vec4(x, y, z, 1.0));

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			normals.push_back(glm::vec3(nx, ny, nz));

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;

		}
	}


	// generate CCW index list of sphere triangles
	std::vector<GLshort> indices;
	int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding first and last stacks
			// k1 => k2 => k1+1
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			// k1+1 => k2 => k2+1
			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	indiceCount = indices.size();

	// VERTEX BUFFERS
	glGenBuffers(1, &masterEntityVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, masterEntityVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), 0, GL_DYNAMIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec4),
		&vertices[0]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// NORMAL BUFFERS
	glGenBuffers(1, &masterEntityNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, masterEntityNormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), 0, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, normals.size() * sizeof(glm::vec3),
		&normals[0]);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// INDEX BUFFERS
	glGenBuffers(1, &masterEntityIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, masterEntityIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLshort), 0, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLshort),
		&indices[0]);

	for (int i = 0; i < sphereCount; i++) {
		Transform trnsf;
		sphereTransforms[i] = trnsf;
	}

	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	float timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

	int k = 0;
	for (int i = 0; i < sphereCount/2; i++) {
		Transform * t = electrons[k].getTransform();
		t->SetPos(glm::vec3(10 * (i), 0, 0));
		t->SetRot(glm::vec3(0, 0.45*(i + 1)*timeElapsed, 0));
		electrons[i].radius = radius;
		sphereTransformMatrices[k++] = t->GetModel();
	}

	for (int i = sphereCount/2; i < sphereCount; i++) {
		Transform * t = electrons[k].getTransform();
		t->SetPos(glm::vec3(10 * (i) - (10*(sphereCount/2)), 0, 12));
		t->SetRot(glm::vec3(0, 0.45*(i + 1)*timeElapsed, 0));
		electrons[i].radius = radius;
		sphereTransformMatrices[k++] = t->GetModel();
	}
	/*for (int i = 0; i < cbrt(sphereCount); ++i) {
		for (int j = 0; j < cbrt(sphereCount); ++j) {
			for (int l = 0; l < cbrt(sphereCount); ++l) {
				Transform * t = electrons[k].getTransform();
				t->SetPos(glm::vec3(10 * (i), 10 * (j), 10 * l));
				t->SetRot(glm::vec3(0, 0.45*(i + 1)*timeElapsed, 0));
				electrons[i].radius = radius;
				sphereTransformMatrices[k++] = t->GetModel();
			}
		}
	}*/

	// TRANSFORM BUFFERS
	glGenBuffers(1, &masterEntityTransformBufferID);

	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glEnableVertexAttribArray(6);
	glEnableVertexAttribArray(7);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
	glVertexAttribDivisor(7, 1);

	// SELECTION/CLICK STATUS BUFFERS
	glGenBuffers(1, &masterEntitySelectionStatusBufferID);
	glEnableVertexAttribArray(8);
	glVertexAttribDivisor(8, 1);

}


// Take the sphereTransforms & sphereTransformsMatrices vectors; make them into Electron vectors which hold the spheretransforms
void entityBufferDraw() {
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	float timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	glm::vec3 sumForces = glm::vec3(0);
	Transform *t1, *t2;
	if (!display.isFrozen()) {
		for (int i = 0; i < sphereCount; ++i)
		{
			t1 = electrons[i].getTransform();
			for (int j = 0; j < sphereCount; ++j) if (j != i)
			{
				t2 = electrons[j].getTransform();

				//if (glm::distance(*(t1->GetPos()), *(t2->GetPos())) < 25.0) {
				sumForces += glm::normalize(*t1->GetPos() - *t2->GetPos()) *  (*electrons[i].getCharge())*(*electrons[j].getCharge()) / pow(glm::distance(*(t1->GetPos()), *(t2->GetPos())), 2);
				//}
			}
			electrons[i].velocity += sumForces / electrons[i].mass;
			*t1->GetPos() += 0.0050f*electrons[i].velocity;
			*t1->GetRot() += electrons[i].charge > 0 ? (glm::vec3(0, 0.001, 0)) : (glm::vec3(0, -0.1, 0));
			sphereTransformMatrices[i] = t1->GetModel();
			sumForces = glm::vec3(0);
		}
	}

	electronShader.Bind();
	glBindBuffer(GL_ARRAY_BUFFER, masterEntityTransformBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * (sphereCount), &sphereTransformMatrices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 0));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 4));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 8));
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * 12));

	glBindBuffer(GL_ARRAY_BUFFER, masterEntitySelectionStatusBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (sphereCount), &sphereSelectionStatuses, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(GLfloat), (void*)0);

	GLuint queryID;
	//glGenQueries();
	glm::mat4 model = camera.GetViewProject();
	glUniformMatrix4fv(*electronShader.getTransfU(), 1, GL_FALSE,
		&model[0][0]);
	glUniform1f(*electronShader.getTimeU(), timeElapsed);
	glDrawElementsInstanced(GL_TRIANGLES, indiceCount, GL_UNSIGNED_SHORT, 0, sphereCount);

	glUseProgram(0);

}


void render()
{
	//Surface surface(50, 50, false, &camera, 100);

	SDL_WarpMouseInWindow(display.getWindow(), 1440 / 2, 900 / 2); // set initial mouse x,y to center of window.
	while (!display.closed()) {

		display.clear(0, 0.025, 0.14 , 0.5);

		std::string ec = (display.isFrozen() ? "PAUSE " : "");
		const char * c = &ec[0];
		entityBufferDraw();


		GLint polygonMode[2];
		glGetIntegerv(GL_POLYGON_MODE, polygonMode);
		if (polygonMode[0] == GL_LINE) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glColor3f(0.8, 0.90, 1.0);
			glLoadIdentity();
			glPushMatrix();
			freetypehelper::print(our_font, 620, 440, c);
			glPopMatrix();

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {

			glColor3f(0.8, 0.90, 1.0);
			glLoadIdentity();
			glPushMatrix();
			freetypehelper::print(our_font, 620, 440, c);
			glPopMatrix();
		}
		display.swapBuffers();

	}
}

int main(int argc, char **argv) {
	bufferSetup();

	render();
	return -1;
}