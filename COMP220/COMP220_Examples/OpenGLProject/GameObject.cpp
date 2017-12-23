#include "GameObject.h"

GameObject::GameObject(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation)
{
	objectPosition = position;
	objectScale = scale;
	objectRotation = rotation;

	loadMeshFromFile("Tank1.FBX", meshes);
	textureID = loadTextureFromFile("Tank1DF.png");
}

GameObject::~GameObject()
{
}

void GameObject::createRigidBody()
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

void GameObject::draw()
{
	for (Mesh *currentMesh : meshes)
	{
		currentMesh->render();
	}
}

void GameObject::update()
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

void GameObject::destroy()
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
}