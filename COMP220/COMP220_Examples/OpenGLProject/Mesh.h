#pragma once

#include <GL\glew.h>
#include <SDL_opengl.h>

#include <vector>

#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void copyMeshData(Vertex * pVerts, unsigned int numberOfVerts, unsigned int * pIndices, unsigned int numberOfIndices);

	void init();
	void destroy();

	void render();

private:
	GLuint m_VBO;
	GLuint m_EBO;

	unsigned int m_numberOfVerts;
	unsigned int m_numberOfIndices;

	GLuint m_VAO;
};

