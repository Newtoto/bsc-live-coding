#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 vertexTextureCoord;
layout (location = 3) in vec3 vertexNormals;

uniform float time = 0.0f;

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0f);

// Camera position
uniform vec3 cameraPosition;

// Lighting
uniform vec3 lightDirection;
uniform vec4 ambientLightColor;
uniform vec4 diffuseLightColor;
uniform vec4 specularLightColor;

// Material
uniform vec4 ambientMaterialColor;
uniform vec4 diffuseMaterialColor;
uniform vec4 specularMaterialColor;
uniform float specularPower;

out vec4 vertexColorOut;
out vec2 vertexTextureCoordOut;
out vec4 diffuse;
out vec4 specular;
out vec4 ambient;

void main()
{
	mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
	vec4 worldNormals = normalize(modelMatrix * vec4(vertexNormals, 0.0f));

	// Model world position
	vec4 worldPosition = modelMatrix * vec4(vertexPosition, 1.0f);

	vec3 viewDirection = normalize(cameraPosition - worldPosition.xyz);

	// Calculate ambient
	ambient = ambientMaterialColor * ambientLightColor;

	// Calculate diffuse lighting
	float nDotL = clamp(dot(worldNormals.xyz, lightDirection), 0, 1);
	diffuse = diffuseMaterialColor * diffuseLightColor * nDotL;

	// Calculate specular lighting
	vec3 halfWay = normalize(lightDirection + viewDirection);
	float nDotH = clamp(dot(worldNormals.xyz, halfWay), 0, 1);
	float specularIntensity = pow(nDotH, specularPower);
	specular = specularMaterialColor * specularLightColor * specularIntensity;

	gl_Position = MVPMatrix * vec4(vertexPosition, 1.0f);
	vertexColorOut = vertexColor;
	vertexTextureCoordOut = vertexTextureCoord;
}
