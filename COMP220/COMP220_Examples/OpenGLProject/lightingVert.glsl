#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec4 vertexColor;
layout (location = 2) in vec2 vertexTextureCoord;
layout (location = 3) in vec3 vertexNormals;

uniform float time = 0.0f;

uniform mat4 modelMatrix = mat4(1.0f);
uniform mat4 viewMatrix = mat4(1.0f);
uniform mat4 projectionMatrix = mat4(1.0f);

// Lighting
uniform vec3 lightDirection;
uniform vec4 diffuseLightColor;

// Material
uniform vec4 diffuseMaterialColor;

out vec4 vertexColorOut;
out vec2 vertexTextureCoordOut;
out vec4 diffuse;

void main()
{
	mat4 MVPMatrix = projectionMatrix * viewMatrix * modelMatrix;
	vec4 worldNormals = normalize(modelMatrix * vec4(vertexNormals, 0.0f));

	// Calculate lighting
	float nDotL = clamp(dot(worldNormals.xyz, lightDirection), 0, 1);
	diffuse = diffuseMaterialColor * diffuseLightColor * nDotL;

	gl_Position = MVPMatrix * vec4(vertexPosition, 1.0f);
	vertexColorOut = vertexColor;
	vertexTextureCoordOut = vertexTextureCoord;
}
