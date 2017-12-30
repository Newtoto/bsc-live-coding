#include "GameObject.h"

GameObject::GameObject()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_modelMatrix = glm::mat4(1.0f);

	m_diffuseMap = 0;

	m_ambientMaterialColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	m_diffuseMaterialColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
	m_specularMaterialColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_specularPower = 25.0f;
	m_shaderProgramID = 0;
}

GameObject::~GameObject()
{
}

// Create game object mesh from file
void GameObject::LoadMeshesFromFile(const std::string& filename)
{
	LoadMeshFromFile(filename, m_meshes);
}

// Create game object texture from file
void GameObject::LoadDiffuseTextureFromFile(const std::string& filename)
{
	m_diffuseMap = LoadTextureFromFile(filename);
}

// Create game object shader program from files
void GameObject::LoadShaderProgram(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	m_shaderProgramID = LoadShaders(vertexShaderFilename.c_str(), fragmentShaderFilename.c_str());
}

void GameObject::CreateRigidBody()
{
	// Create colision
	objectColShape = new btBoxShape(btVector3(2, 2, 2));
	// Create Dynamic Objects
	objectTransform.setIdentity();
	objectTransform.setOrigin(btVector3(m_position.x, m_position.y, m_position.z));
	btVector3 objectInertia(0, 0, 0);

	btScalar objectMass(1.f);

	objectColShape->calculateLocalInertia(objectMass, objectInertia);

	btDefaultMotionState* myMotionState = new btDefaultMotionState(objectTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(objectMass, myMotionState, objectColShape, objectInertia);
	m_rigidBody = new btRigidBody(rbInfo);
}

void GameObject::PreRender()
{
	glUseProgram(m_shaderProgramID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_diffuseMap);

	GLint modelMatrixLocation = glGetUniformLocation(m_shaderProgramID, "modelMatrix");
	GLint textureLocation = glGetUniformLocation(m_shaderProgramID, "baseTexture");

	ambientMaterialColorLocation = glGetUniformLocation(m_shaderProgramID, "ambientMaterialColor");
	diffuseMaterialColorLocation = glGetUniformLocation(m_shaderProgramID, "diffuseMaterialColor");
	specularMaterialColorLocation = glGetUniformLocation(m_shaderProgramID, "specularMaterialColor");
	specularPowerLocation = glGetUniformLocation(m_shaderProgramID, "specularPower");

	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, value_ptr(m_modelMatrix));

	glUniform1i(textureLocation, 0);

	glUniformMatrix4fv(ambientMaterialColorLocation, 1, GL_FALSE, value_ptr(m_ambientMaterialColor));
	glUniformMatrix4fv(diffuseMaterialColorLocation, 1, GL_FALSE, value_ptr(m_diffuseMaterialColor));
	glUniformMatrix4fv(specularMaterialColorLocation, 1, GL_FALSE, value_ptr(m_specularMaterialColor));
	glUniform1f(specularPowerLocation, m_specularPower);

}

void GameObject::Render()
{
	for (Mesh *currentMesh : m_meshes)
	{
		currentMesh->Render();
	}
}

void GameObject::Update()
{
	if (m_rigidBody)
	{
		// Update physics
		objectTransform = m_rigidBody->getWorldTransform();
		objectOrigin = objectTransform.getOrigin();
		objectPhysicsRotation = objectTransform.getRotation();

		m_position = glm::vec3(objectOrigin.getX(), objectOrigin.getY(), objectOrigin.getZ());
	}

	m_rotationMatrix = rotate(m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f))*rotate(m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f))*rotate(m_rotation.z, glm::vec3(1.0f, 0.0f, 1.0f));
	m_scaleMatrix = scale(m_scale);
	m_translationMatrix = translate(m_position);

	m_modelMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

void GameObject::Destroy()
{
	// Delete rigid body
	delete objectColShape;
	delete m_rigidBody->getMotionState();
	delete m_rigidBody;

	// Delete diffuse map memory
	glDeleteTextures(1, &m_diffuseMap);

	// Delete program ID memory
	glDeleteProgram(m_shaderProgramID);

	// Delete meshes
	auto iter = m_meshes.begin();
	while (iter != m_meshes.end())
	{
		if ((*iter))
		{
			delete (*iter);
			iter = m_meshes.erase(iter);
		}
		else
		{
			iter++;
		}
	}
	m_meshes.clear();
}