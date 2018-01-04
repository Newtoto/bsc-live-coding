#version 330 core

in vec4 vertexColorOut;
in vec2 vertexTextureCoordOut;
in vec4 diffuse;
in vec4 specular;
in vec4 ambient;

out vec4 color;

uniform vec4 fragColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform sampler2D baseTexture;

void main(){
	color = ambient + (diffuse*texture(baseTexture, vertexTextureCoordOut)) + specular;
	//color = ambient + (diffuse*texture(baseTexture, vertexTextureCoordOut)) + specular;
	//color = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
