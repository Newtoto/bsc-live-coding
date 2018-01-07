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
#include "Camera.h"

class GameObject
{
public:
	class GameObject();
	~GameObject();

	void LoadMeshesFromFile(const std::string& filename);
	void LoadDiffuseTextureFromFile(const std::string& filename);
	void LoadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	void CreateRigidBody();
	void PreRender();
	void Render();
	void Update();
	void Destroy();
	void Move(btVector3 movementAmount);

	btRigidBody* m_rigidBody;

	// --- START OF SETTERS AND GETTERS ---
	
	// Position of the game object
	// Set position
	void SetPosition(const glm::vec3& position)
	{
		m_position = position;
	};
	// Get position
	const glm::vec3& GetPosition()
	{
		return m_position;
	};

	// Scale of the game object
	// Set scale
	void SetScale(const glm::vec3& scale)
	{
		m_scale = scale;
	};
	// Get scale
	const glm::vec3& GetScale()
	{
		return m_scale;
	};

	// Rotation of the game object
	// Set rotation
	void SetRotation(const glm::vec3& rotation)
	{
		m_rotation = rotation;
	};
	// Get rotation
	const glm::vec3& GetRotation()
	{
		return m_rotation;
	};

	// Model matrix of game object
	// Get model matrix
	const glm::mat4& GetModelMatrix()
	{
		return m_modelMatrix;
	};

	// Diffuse map of the game object
	// Get diffuse map
	const GLuint GetDiffuseMap()
	{
		return m_diffuseMap;
	};

	// Ambient material color of the game object
	// Set ambient material color
	void SetAmbientMaterialColor(const glm::vec4& color)
	{
		m_ambientMaterialColor = color;
	};
	// Get ambient material color
	const glm::vec4& GetAmbientMaterialColor()
	{
		return m_ambientMaterialColor;
	};

	// Diffuse material color of the game object
	// Set diffuse material color
	void SetDiffuseMaterialColor(const glm::vec4& color)
	{
		m_diffuseMaterialColor = color;
	};
	// Get diffuse material color
	const glm::vec4& GetDiffuseMaterialColor()
	{
		return m_diffuseMaterialColor;
	};

	// Specular material color of the game object
	// Set specular material color
	void SetSpecularMaterialColor(const glm::vec4& color)
	{
		m_specularMaterialColor = color;
	};
	// Get specular material color
	const glm::vec4& GetSpecularMaterialColor()
	{
		return m_specularMaterialColor;
	};

	// Specular power of the game object
	// Set specular power
	void SetSpecularPower(const float power)
	{
		m_specularPower = power;
	};
	// Get specular power
	const float GetSpecularPower()
	{
		return m_specularPower;
	};

	// Shader program ID of the game object
	// Get shader program ID
	const GLuint GetShaderProgramID()
	{
		return m_shaderProgramID;
	};

	// --- END OF SETTERS AND GETTERS ---

private:

	// Uniform locations
	GLint modelMatrixLocation;
	GLint textureLocation;

	GLint ambientMaterialColorLocation;
	GLint diffuseMaterialColorLocation;
	GLint specularMaterialColorLocation;
	GLint specularPowerLocation;

	// Visible mesh
	std::vector<Mesh*> m_meshes;

	// Textures
	GLuint m_diffuseMap;

	// Transform
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;

	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;
	glm::mat4 m_translationMatrix;
	glm::mat4 m_modelMatrix;

	// Material
	glm::vec4 m_ambientMaterialColor;
	glm::vec4 m_diffuseMaterialColor;
	glm::vec4 m_specularMaterialColor;
	float m_specularPower;

	// Bullet physics

	btTransform objectTransform;
	btCollisionShape* objectColShape;
	btVector3 objectOrigin;
	btQuaternion objectPhysicsRotation;

	GLuint m_shaderProgramID;
};

