#include "Lighting.h"



Lighting::Lighting()
{
	direction = glm::vec3(0.0f, 0.0f, -1.0f);
	ambientColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	diffuseColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specularColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	specularPower = 25.0f;
}


Lighting::~Lighting()
{
}
