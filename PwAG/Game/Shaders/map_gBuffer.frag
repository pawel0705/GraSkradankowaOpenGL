#version 430 

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main()
{
	gPosition = FragPos;
	gNormal = normalize(Normal);
	gAlbedoSpec.rgb = texture(diffuse, TexCoords).rgb;
	gAlbedoSpec.a = texture(specular, TexCoords).r;
}