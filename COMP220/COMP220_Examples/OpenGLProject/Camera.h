#pragma once

#include <string>

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

	void moveView(float windowWidth, float windowHeight);
	void forward(float magnitude);
	void sideways(float magnitude);
	
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
};