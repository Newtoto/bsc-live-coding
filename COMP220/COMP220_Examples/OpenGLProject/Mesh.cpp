#include "Mesh.h"

Mesh::Mesh()
{
	m_VBO = 0;
	m_EBO = 0;
	m_VAO = 0;
	m_numberOfVerts = 0;
	m_numberOfIndices = 0;
}


Mesh::~Mesh()
{
}

void Mesh::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
}

void Mesh::CopyMeshData(Vertex * pVerts, unsigned int numberOfVerts, unsigned int * pIndices, unsigned int numberOfIndices)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, numberOfVerts * sizeof(Vertex), pVerts, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfIndices * sizeof(unsigned int), pIndices, GL_STATIC_DRAW);

	m_numberOfVerts = numberOfVerts;
	m_numberOfIndices = numberOfIndices;
	glBindVertexArray(m_VAO);
	//1st attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,					//Attribute 0, no reason for 0, but must match layout in the shader
		3,					//Size 
		GL_FLOAT,			//Type
		GL_FALSE,			//Normalized?
		sizeof(Vertex),		//Stride
		(void*)0			//Array buffer offset
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(7 * sizeof(float)));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(9 * sizeof(float)));
}

void Mesh::Destroy()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
}

void Mesh::Render()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);


	glDrawElements(GL_TRIANGLES, m_numberOfIndices, GL_UNSIGNED_INT, (void*)0);
}
