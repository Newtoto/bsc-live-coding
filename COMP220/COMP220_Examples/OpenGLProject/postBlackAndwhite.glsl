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
	if (luminescence < 0.08)
	{
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
	else
	{
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}
}
