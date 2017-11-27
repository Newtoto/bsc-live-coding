#include "GameObject.h"

GameObject::GameObject()
{
	objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objectRotation = glm::vec3(0.0f, 0.0f, 0.0f);
}


GameObject::~GameObject()
{
}

void GameObject::init()
{
	objectPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	objectScale = glm::vec3(1.0f, 1.0f, 1.0f);
	objectRotation = glm::vec3(0.0f, 0.0f, 0.0f);

	loadMeshFromFile("Tank1.FBX", meshes);
	textureID = loadTextureFromFile("Tank1DF.png");
}

void GameObject::draw()
{
	for (Mesh *currentMesh : meshes)
	{
		currentMesh->render();
	}
}

void GameObject::update()
{
	rotationMatrix = rotate(objectRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))*rotate(objectRotation.y, glm::vec3(0.0f, 1.0f, 0.0f))*rotate(objectRotation.z, glm::vec3(1.0f, 0.0f, 1.0f));
	scaleMatrix = scale(objectScale);
	translationMatrix = translate(objectPosition);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::destroy()
{
	auto iter = meshes.begin();
	while (iter != meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	meshes.clear();

	glDeleteTextures(1, &textureID);
}