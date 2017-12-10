#pragma once

#include <string>
#include <vector>
#include <fstream>

#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>

#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

#include "Vertex.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "Mesh.h"

class GameObject
{
public:
	class GameObject();
	~GameObject();

	void init();
	void createRigidBody();
	void draw();
	void update();
	void destroy();

	glm::vec3 objectPosition;
	glm::vec3 objectScale;
	glm::vec3 objectRotation;

	std::vector<Mesh*> meshes;
	
	GLuint textureID;

	glm::mat4 modelMatrix;

	btRigidBody* objectRigidBody;


private:
	glm::mat4 rotationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 translationMatrix;

	// Bullet physics
	btTransform objectTransform;
	btCollisionShape* objectColShape;
	btVector3 objectOrigin;
	btQuaternion objectPhysicsRotation;
};

