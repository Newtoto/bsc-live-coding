#version 330 core

in vec2 textureCoordsOut;

out vec4 color;

uniform sampler2D texture0;

float getLuminescence(vec3 color)
{
	return (color.r*color.g*color.b)/3.0f;
}

void main(){
	vec4 textureColor = texture(texture0, textureCoordsOut);
	float luminescence = getLuminescence(textureColor.rgb);
	float averageTone = (textureColor.r + textureColor.g + textureColor.b)/3.0f;
	color = vec4(averageTone + 0.44f, averageTone + 0.26f, averageTone + 0.08f, 1.0f);
}
