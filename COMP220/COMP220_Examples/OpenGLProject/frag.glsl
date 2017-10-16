#version 330 core

//in vec4 vertexColorOut;

out vec4 color;

uniform vec4 fragColor = vec4(1.0, 1.0, 1.0, 1.0);

void main(){
	color = fragColor;
	//color = vertexColorOut;
}
