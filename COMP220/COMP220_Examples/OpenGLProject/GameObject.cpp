#include "GameObject.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	objectPosition = position;
	objectScale = scale;
	objectRotation = rotation;

	LoadMeshFromFile("Tank1.FBX", meshes);
	textureID = LoadTextureFromFile("Tank1DF.png");

	programID = LoadShaders("lightingVert.glsl", "lightingFrag.glsl");
}

GameObject::~GameObject()
{
}

void GameObject::CreateRigidBody()
{
	// Create colision
	objectColShape = new btBoxShape(btVector3(2, 2, 2));
	// Create Dynamic Objects
	objectTransform.setIdentity();
	objectTransform.setOrigin(btVector3(objectPosition.x, objectPosition.y, objectPosition.z));
	btVector3 objectInertia(0, 0, 0);

	btScalar objectMass(1.f);

	objectColShape->calculateLocalInertia(objectMass, objectInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(objectTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(objectMass, myMotionState, objectColShape, objectInertia);
	objectRigidBody = new btRigidBody(rbInfo);
}

void GameObject::CreateUniformLocations()
{
	fragColorLocation = glGetUniformLocation(programID, "fragColor");
	if (fragColorLocation < 0)
	{
		printf("Unable to find %s uniform", "fragColor");
	}

	currentTimeLocation = glGetUniformLocation(programID, "time");
	if (currentTimeLocation < 0)
	{
		printf("Unable to find %s uniform", "time");
	}

	modelMatrixLocation = glGetUniformLocation(programID, "modelMatrix");
	if (modelMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "modelMatrix");
	}
	viewMatrixLocation = glGetUniformLocation(programID, "viewMatrix");
	if (viewMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "viewMatrix");
	}
	projectionMatrixLocation = glGetUniformLocation(programID, "projectionMatrix");
	if (projectionMatrixLocation < 0)
	{
		printf("Unable to find %s uniform", "projectionMatrix");
	}
	textureLocation = glGetUniformLocation(programID, "baseTexture");
	if (textureLocation < 0)
	{
		printf("Unable to find %s uniform", "baseTexture");
	}
	cameraPositionLocation = glGetUniformLocation(programID, "cameraPosition");
	if (cameraPositionLocation < 0)
	{
		printf("Unable to find %s uniform", "cameraPosition");
	}
}

void GameObject::UseUniformLocations(float currentTicks, Camera playerCamera)
{
	glUniform1f(currentTimeLocation, (float)(currentTicks) / 1000.0f);

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, value_ptr(playerCamera.viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, value_ptr(playerCamera.projectionMatrix));

	glUniform3fv(cameraPositionLocation, 1, value_ptr(playerCamera.cameraPosition));
	glUniform1i(textureLocation, 0);
}

void GameObject::Draw()
{
	for (Mesh *currentMesh : meshes)
	{
		currentMesh->Render();
	}
}

void GameObject::Update()
{
	if (objectRigidBody)
	{
		// Update physics
		objectTransform = objectRigidBody->getWorldTransform();
		objectOrigin = objectTransform.getOrigin();
		objectPhysicsRotation = objectTransform.getRotation();

		objectPosition = glm::vec3(objectOrigin.getX(), objectOrigin.getY(), objectOrigin.getZ());
	}

	rotationMatrix = rotate(objectRotation.x, glm::vec3(1.0f, 0.0f, 0.0f))*rotate(objectRotation.y, glm::vec3(0.0f, 1.0f, 0.0f))*rotate(objectRotation.z, glm::vec3(1.0f, 0.0f, 1.0f));
	scaleMatrix = scale(objectScale);
	translationMatrix = translate(objectPosition);

	modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
}

void GameObject::Destroy()
{
	// Delete rigid body
	delete objectColShape;
	delete objectRigidBody->getMotionState();
	delete objectRigidBody;

	// Delete meshes
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
	glDeleteProgram(programID);
}