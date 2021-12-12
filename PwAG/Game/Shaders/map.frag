#version 430 

uniform sampler2D diffuse;
uniform sampler2D normalMap;

//uniform vec3 ambientLight;
uniform vec3 cameraPos;

in vec3 v_Color;
in vec2 v_TextCoord;
in vec3 v_Normal;
in vec3 v_Position;

in vec3 v_Offset;

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

#define MAX_SPOT_LIGHT_COUNT 16

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	if (gl_FrontFacing) {
		
		vec3 norm = normalize(v_Normal);
		vec3 viewDir = normalize(cameraPos - v_Position);

		vec3 result = vec3(0, 0, 0);
		for(int i = 0; i < pointLightsCount; ++i)
		{
			result += calculatePointLight(pointLights[i], norm, v_Position, viewDir);
		}

		gl_FragColor = vec4(result, 1.0);
	}
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 posToLightDirectionVector  = normalize(light.position - fragPos);

	// diffuse light
	normal = normal + texture(normalMap, v_TextCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	float diffuseLight = max(dot(normal, posToLightDirectionVector ), 0.0);

	// specular light
	vec3 reflectDirVec = reflect(-posToLightDirectionVector , normal);
	float specularConstant = pow(max(dot(viewDir, reflectDirVec), 0.0), 105);
	//vec3 halfwayDir = normalize(posToLightDirectionVector + viewDir);
	//float specularConstant = pow(max(dot(normal, halfwayDir), 0.0), 205);

	// attentuation
	float distance = length(light.position - v_Position);
	float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	vec3 textured = texture(diffuse, v_TextCoord).rgb;
	
	vec3 ambientFinal = light.ambient * textured;
	vec3 diffuseFinal = light.diffuse * diffuseLight * textured;
	vec3 specularFinal = light.specular * specularConstant * textured;

	ambientFinal *= attentuation;
	diffuseFinal *= attentuation;
	specularFinal *= attentuation;

	return ambientFinal + diffuseFinal + specularFinal;
}