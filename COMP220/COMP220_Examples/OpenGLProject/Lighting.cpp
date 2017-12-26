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

void Lighting::InitialiseUniformLocations(GLuint programID) {
	// Lighting
	lightDirectionLocation = glGetUniformLocation(programID, "lightDirection");
	if (lightDirectionLocation < 0)
	{
		printf("Unable to find %s uniform", "lightDirection");
	}
	ambientLightColorLocation = glGetUniformLocation(programID, "ambientLightColor");
	if (ambientLightColorLocation < 0)
	{
		printf("Unable to find %s uniform", "ambientLightColor");
	}
	diffuseLightColorLocation = glGetUniformLocation(programID, "diffuseLightColor");
	if (diffuseLightColorLocation < 0)
	{
		printf("Unable to find %s uniform", "diffuseLightColor");
	}
	specularLightColorLocation = glGetUniformLocation(programID, "specularLightColor");
	if (specularLightColorLocation < 0)
	{
		printf("Unable to find %s uniform", "specularLightColor");
	}
	specularPowerLocation = glGetUniformLocation(programID, "specularPower");
	if (specularPowerLocation < 0)
	{
		printf("Unable to find %s uniform", "specularMaterialPower");
	}
}

//void Lighting::AttatchUniformLocations(GLuint programID)
//{
//	glUseProgram(programID);
//
//	glUniform3fv(lightDirectionLocation, 1, value_ptr(direction));
//	glUniform4fv(ambientLightColorLocation, 1, value_ptr(ambientColor));
//	glUniform4fv(diffuseLightColorLocation, 1, value_ptr(diffuseColor));
//	glUniform4fv(specularLightColorLocation, 1, value_ptr(specularColor));
//	glUniform1f(specularPowerLocation, specularPower);
//}