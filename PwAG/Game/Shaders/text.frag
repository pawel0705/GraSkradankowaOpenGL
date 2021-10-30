#version 430

in vec2 texCoords;
out vec4 color;

uniform sampler2D samp;
uniform vec3 textColor;

void main()
{
	color = vec4(textColor.rgb, texture(samp, texCoords).r);
}