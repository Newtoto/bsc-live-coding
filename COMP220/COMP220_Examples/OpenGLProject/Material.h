#pragma once
#include <fstream>

#include <glm\glm.hpp>
#include <GL\glew.h>
#include <glm\gtc\type_ptr.hpp>

#include "Lighting.h"

class Material
{
public:
	Material();
	~Material();

	void InitialiseUniformLocations(GLuint programID);
	void UseUniformLocations(GLuint programID, Lighting light);

	glm::vec4 ambientMaterialColor;
	glm::vec4 diffuseMaterialColor;
	glm::vec4 specualarMaterialColor;

	GLint ambientMaterialColorLocation;
	GLint diffuseMaterialColorLocation;
	GLint specularMaterialColorLocation;
};


