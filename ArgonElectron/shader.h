#ifndef SHADER_INCLUDED_H
#define SHADER_INCLUDED_H

#include <string>
#include "transform.h"
#include <glew.h>
#include "camera.h"

class Shader
{
public:
	Shader(const std::string& fileName);

	void Bind();
	//Updates uniforms in our uniform array 
	GLuint * Program() { return &main_program; }

	void Update(const Transform& transform, const Camera& camera)
	{

		glm::mat4 MVP = camera.GetViewProject() * transform.GetModel();
		glm::mat4 Normal = transform.GetModel();

		glUniformMatrix4fv(m_uniforms[0], 1, GL_FALSE, &MVP[0][0]);
		glUniformMatrix4fv(m_uniforms[1], 1, GL_FALSE, &Normal[0][0]);
		glUniform3f(m_uniforms[2], 0.0f, 0.0f, 1.0f);
	};

	void Update(glm::mat4 modelmatrix, const Camera& camera)
	{	

		glm::mat4 model = camera.GetViewProject()*modelmatrix;

		glUniformMatrix4fv(m_uniforms[TRANSFORM_U], 1, GL_FALSE, &model[0][0]);
	
	};

	GLuint * getTransfU() { return &m_uniforms[TRANSFORM_U]; }
	GLuint * getTimeU() { return &m_uniforms[TIME_U]; }
	GLuint * getPositionsU() { return &m_uniforms[POSITIONS_U]; }
	GLuint * getCameraPositionU() { return &m_uniforms[CAMERAPOSITION_U]; }

	virtual ~Shader();
protected:
private:
	static const unsigned int NUM_SHADERS = 2;

	void operator=(const Shader& shader) {}
	Shader(const Shader& shader) {}

	std::string LoadShader(const std::string& fileName);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	GLuint CreateShader(const std::string& text, unsigned int type);

	enum
	{
		TRANSFORM_U, //U for uniform
		CAMERAPOSITION_U,
		TIME_U,
		POSITIONS_U,
		NUM_UNIFORMS
	};

	GLuint main_program;
	GLuint m_shaders[NUM_SHADERS];
	GLuint m_uniforms[NUM_UNIFORMS];
};

#endif