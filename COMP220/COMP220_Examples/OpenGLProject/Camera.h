#pragma once

#include <SDL.h>

#include <glm\glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\transform.hpp>
#include <glm\gtc\type_ptr.hpp>

class Camera
{
public:
	class Camera();
	~Camera();

	void update(float windowWidth, float windowHeight);

	glm::vec3 cameraPosition;
	float mouseSensitivity;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;

private:
	// default camera position
	
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;

	glm::vec3 position;
	float horizontalAngle;
	float verticalAngle;
	float fieldOfView;

	float cameraSpeed;
};