#version 430 

uniform sampler2D diffuse;

uniform vec3 cameraPos;

in vec3 v_Color;
in vec2 v_TextCoord;
in vec3 v_Normal;
in vec3 v_Position;

struct PointLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_POINT_LIGHT_COUNT 64

uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightsCount;

void main()
{
	if (gl_FrontFacing) {
		vec3 result = vec3(0.0, 0.0, 0.0);
		for(int i = 0; i < pointLightsCount; ++i)
		{
			vec3 textured = texture(diffuse, v_TextCoord).rgb;

			// ambient light
			vec3 ambientFinal = pointLights[i].ambient * textured;
	
			// attentuation
			float distance = length(pointLights[i].position - v_Position);
			// constant linear quadric
			float attentuation = 1.0 / (pointLights[i].constant + pointLights[i].linear * distance + pointLights[i].quadratic * (distance * distance));

			ambientFinal *= attentuation;
			result += ambientFinal;
		}	
		gl_FragColor = vec4(result, 1.0);
	}
}