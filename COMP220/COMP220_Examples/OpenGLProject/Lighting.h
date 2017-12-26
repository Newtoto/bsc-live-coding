#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
class Lighting
{
public:
	Lighting();
	~Lighting();

	glm::vec3 direction;
	glm::vec4 ambientColor;
	glm::vec4 diffuseColor;
	glm::vec4 specularColor;
	float specularPower;
};

