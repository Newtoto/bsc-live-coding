#include "Camera.h"

Camera::Camera()
{
	// default camera position
	cameraPosition = glm::vec3(0.0f, 2.0f, -5.0f);
	cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	playerHeight = 2.5f;

	viewMatrix = lookAt(cameraPosition, cameraTarget, cameraUp);

	position = cameraPosition;
	horizontalAngle = 0.0f;
	verticalAngle = 0.0f;
	fieldOfView = 45.0f;

	cameraSpeed = 0.1f;
	mouseSensitivity = 0.01;

	projectionMatrix = glm::perspective(glm::radians(90.0f), float(4 / 3), 0.1f, 100.0f);

	fallTime = 0;

	jumping = false;
	flyingReady = false;
	flying = false;
}


Camera::~Camera()
{
}

void Camera::MoveView(float windowWidth, float windowHeight)
{
	// Get mouse movement
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	// Move camera based on mouse movement
	horizontalAngle += mouseSensitivity * float(windowWidth / 2 - mouseX);
	verticalAngle += mouseSensitivity * float(windowHeight / 2 - mouseY);
	verticalAngle = glm::clamp(verticalAngle, -0.5f, 0.7f);
	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

	//Recalculate camera
	viewMatrix = lookAt(cameraPosition, direction + cameraPosition, cameraUp);
}

void Camera::ApplyGravity(float groundHeight)
{
	headHeight = groundHeight + playerHeight;
	if (cameraPosition.y > headHeight)
	{
		// only apply gravity when player is not flying
		if (!flying)
		{
			// Limit terminal velocity
			if (fallTime < 1000)
			{
				fallTime += 9.8;
			}
			cameraPosition.y -= 0.001 * fallTime;
		}
	}
	else
	{
		// if not above ground height, stop jumping and flying
		jumping = false;
		flyingReady = false;
		flying = false;

		fallTime = 0;
	}
}

void Camera::Jump()
{
	if (!jumping)
	{
		// stop player from inputting jump until jumping is false
		jumpTimer = 10;
		jumping = true;
	}
	else
	{
		// allow the player to start flying by double pressing space
		if (flyingReady)
		{
			printf("flying now");
			flying = true;
		}
	}
	if (jumpTimer > 0)
	{
		// for 10 ticks, move the player upward from jumping force
		jumpTimer -= 1;
		cameraPosition.y += 0.8 * jumpTimer/10;
	}
}

void Camera::Fly(float magnitude)
{
	// move player up and down if flying
	if (flying)
	{
		// Make player move faster
		cameraSpeed = 0.25f;
		// Move camera vertically by fly input magnitude
		cameraPosition.y += 0.1 * magnitude;
	}
	else
	{
		// Make player speed normal
		cameraSpeed = 0.1f;
	}
}

void Camera::Forward(float magnitude)
{
	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

	cameraPosition += direction * glm::vec3(magnitude * cameraSpeed, 0.0f, magnitude * cameraSpeed);
}

void Camera::Sideways(float magnitude)
{
	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

	cameraPosition.x += direction.z * magnitude * cameraSpeed;
	cameraPosition.z -= direction.x * magnitude * cameraSpeed;
}

void Camera::AdjustMouseSensitivity(float magnitude)
{
	if (magnitude < 0 && mouseSensitivity > 0.001)
	{
		// Adjust mouse sensitivity down, but not below 0
		mouseSensitivity += 0.001 * magnitude;
	}
	else if (magnitude > 0 && mouseSensitivity < 2)
	{
		mouseSensitivity += 0.001* magnitude;
	}

	printf("%", mouseSensitivity);
}