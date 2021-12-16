#version 430 

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 v_TextCoord;
in vec3 v_Position;
in vec3 v_Normal;

in mat3 TBN;

uniform sampler2D diffuse;
uniform sampler2D specular;

void main()
{
	gPosition = v_Position;
	gNormal = normalize(v_Normal);
	gAlbedoSpec.rgb = texture(diffuse, v_TextCoord).rgb;
	gAlbedoSpec.a = texture(specular, v_TextCoord).r;
}