#include "Material.h"



Material::Material()
{
	ambientMaterialColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
	diffuseMaterialColor = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
	specualarMaterialColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


Material::~Material()
{
}

void Material::InitialiseUniformLocations(GLuint programID) {
	ambientMaterialColorLocation = glGetUniformLocation(programID, "ambientMaterialColor");
	if (ambientMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "ambientMaterialColor");
	}
	diffuseMaterialColorLocation = glGetUniformLocation(programID, "diffuseMaterialColor");
	if (diffuseMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "diffuseMaterialColor");
	}
	specularMaterialColorLocation = glGetUniformLocation(programID, "specularMaterialColor");
	if (specularMaterialColorLocation < 0)
	{
		printf("Unable to find %s uniform", "specularMaterialColor");
	}
}

void Material::UseUniformLocations(GLuint programID, Lighting light)
{
	glUseProgram(programID);

	glUniform4fv(ambientMaterialColorLocation, 1, value_ptr(ambientMaterialColor));
	glUniform4fv(diffuseMaterialColorLocation, 1, value_ptr(diffuseMaterialColor));
	glUniform4fv(specularMaterialColorLocation, 1, value_ptr(light.specularColor));
}