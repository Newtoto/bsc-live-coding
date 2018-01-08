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

	// Moves camera based on mouse movement
	void MoveView(float windowWidth, float windowHeight);
	// Moves camera down by an increasing amount of not on the ground
	void ApplyGravity(float groundHeight);
	// Moves the camera up over time
	void Jump();
	// Moves the camera up and down
	void Fly(float magnitude);
	// Moves the camera the way it's facing
	void Forward(float magnitude);
	// Moves the camera 90 degrees to the way it's facing
	void Sideways(float magnitude);
	// Increases or decreases the amount the mouse moves the camera
	void AdjustMouseSensitivity(float magnitude);
	
	glm::vec3 cameraPosition;

	float mouseSensitivity;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;

	float horizontalAngle;
	float verticalAngle;
	float headHeight;

	float fallTime;

	float jumpTimer;
	bool jumping;
	bool flyingReady;
	bool flying;

private:
	// default camera position
	
	glm::vec3 cameraTarget;
	glm::vec3 cameraUp;

	glm::vec3 position;
	float fieldOfView;

	float cameraSpeed;

	float playerHeight;
};