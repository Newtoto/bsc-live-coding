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

	void CopyMeshData(Vertex * pVerts, unsigned int numberOfVerts, unsigned int * pIndices, unsigned int numberOfIndices);

	void Init();
	void Destroy();

	void Render();

private:
	GLuint m_VBO;
	GLuint m_EBO;

	unsigned int m_numberOfVerts;
	unsigned int m_numberOfIndices;

	GLuint m_VAO;
};

