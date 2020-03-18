#include "stdafx.h"
#include "surface.h"


Surface::Surface(unsigned int w, unsigned int l, bool f, Camera * c, unsigned int scale)
{

	// MEMBER INITIALIZATIONS
	width = w; length = l; flat = f;
	shader = new Shader("./shaders/gridShader");
	camera = c;
	transform = Transform(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
	transform.SetPos(glm::vec3(0, -10, 0));
	transform.SetRot(glm::vec3(0, 0, 0));

	// Algorithm for generating the plane's vertex data
	if (flat)
	{
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; i++)
				positions.push_back(glm::vec4((float)i*scale, 0, ((float)j*scale), 1));
	}

	else
	{
		srand(3);
		for (int j = 0; j <= width; j++)
			for (int i = 0; i <= length; ++i)
			{
				float rnd = rand() % 100 + 1;
				positions.push_back(glm::vec4(((float)i)*scale, (float)(rand() % 8) / 5, ((float)j)*scale, 1));
			}
	}

	for (int i = 0; i < length; i++)
	{
		indices.push_back(i + length + 1);
		indices.push_back(i + length + 2);
		indices.push_back(i + 1);
		indices.push_back(i + 1);
		indices.push_back(i);
		indices.push_back(i + length + 1);
	}

	for (int k = 1; k < width; k++) // Generate indices (valid for GL_TRIANGLES draw mode) // To review - generates 1 extra set of indices 
		for (int i = k * length + k; i < (k + 1)*length + k; i++)
		{
			indices.push_back(i + length + 1);
			indices.push_back(i + length + 2);
			indices.push_back(i + 1);
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(i + length + 1);
		}

	glGenBuffers(1, vbo);
	glGenBuffers(1, vbo2);
	glGenBuffers(1, ebo);
	glGenBuffers(1, vao);

}

void Surface::Draw()
{

	glBindBuffer(GL_ARRAY_BUFFER, *vao);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec4), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, *vbo2);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLshort), indices.data(), GL_STATIC_DRAW);
	
	shader->Bind();
	shader->Update(transform, *camera);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);


}
Surface::~Surface()
{
}
