#pragma once
#include <fstream>

#include <glm\glm.hpp>
#include <GL\glew.h>

class Lighting
{
public:
	Lighting();
	~Lighting();

	void InitialiseUniformLocations(GLuint programID);
	void AttatchUniformLocations(GLuint programID);

	glm::vec3 direction;
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularPower;

	GLint lightDirectionLocation;
	GLint ambientLightColorLocation;
	GLint diffuseLightColorLocation;
	GLint specularLightColorLocation;
	GLint specularPowerLocation;
};

