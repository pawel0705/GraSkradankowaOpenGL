#version 430

layout (location = 0) in vec4 vertex;
out vec2 texCoords;

uniform mat4 MVP;

void main()
{
	gl_Position = MVP * vec4(vertex.xy, 0.0, 1.0);
	texCoords = vec2(vertex.zw);
}