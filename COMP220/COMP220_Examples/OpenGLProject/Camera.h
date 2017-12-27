#pragma once

#include <string>

#include <SDL.h>

#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include <btBulletDynamicsCommon.h>

class Camera
{
public:
	class Camera();
	~Camera();

	void MoveView(float windowWidth, float windowHeight);
	void ApplyGravity(float groundHeight);
	void Forward(float magnitude);
	void Sideways(float magnitude);
	
	glm::vec3 cameraPosition;

	float mouseSensitivity;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;

	float horizontalAngle;
	float verticalAngle;

private:
	// default camera position
	
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;

	glm::vec3 position;
	float fieldOfView;

	float cameraSpeed;

	float playerHeight;
};