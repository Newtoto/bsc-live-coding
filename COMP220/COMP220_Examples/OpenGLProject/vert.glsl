#version 330 core

layout (location = 0) in vec3 vertexPosition;

uniform float time = 0.0f;

uniform mat4 modelMatrix = mat4(1.0f);

void main(){
	vec4 modelVertexPosition = modelMatrix*vec4(vertexPosition, 1.0f);

	gl_Position = modelVertexPosition;
}
