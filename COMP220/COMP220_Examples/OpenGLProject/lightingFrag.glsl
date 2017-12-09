#version 330 core

in vec4 vertexColorOut;
in vec2 vertexTextureCoordOut;
in vec4 diffuse;

out vec4 color;

uniform vec4 fragColor = vec4(1.0, 1.0, 1.0, 1.0);

uniform sampler2D baseTexture;

void main(){
	color = diffuse*texture(baseTexture, vertexTextureCoordOut);
}
