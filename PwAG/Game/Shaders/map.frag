#version 430 

uniform sampler2D diffuse;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
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

struct SpotLight
{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define MAX_POINT_LIGHT_COUNT 16

uniform PointLight pointLights[MAX_POINT_LIGHT_COUNT];
uniform int pointLightsCount;

#define MAX_POINT_LIGHT_COUNT 16

uniform SpotLight spotLights[MAX_POINT_LIGHT_COUNT];
uniform int spotLightsCount;

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

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
		
		for(int i = 0; i < spotLightsCount; ++i)
		{
			result += calculateSpotLight(spotLights[i], norm, v_Position, viewDir);
		}

		gl_FragColor = vec4(result, 1.0);
	}
}

vec3 calculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// diffuse light
	vec3 posToLightDirectionVector = normalize(lightPosition - fragPos);
	//float diffuseLight = clamp(dot(posToLightDirectionVector, normalize(v_Normal)), 0, 1);
	float diffuseLight = max(dot(normal, posToLightDirectionVector), 0.0f);
	vec3 diffuseFinal = light.diffuse * diffuseLight;
	
	// specular light
	vec3 lightToPosDirectionVector = normalize(fragPos - lightPosition);
	vec3 reflectDirVec = normalize(reflect(lightToPosDirectionVector, normal));
	vec3 posToViewDirVec = normalize(cameraPos - fragPos);
	float specularConstant = pow(max(dot(viewDir, reflectDirVec), 0.0), 105);
	vec3 specularFinal = light.diffuse * specularConstant * texture(diffuse, v_TextCoord).rgb;
	
	// ambient light
	vec3 ambientFinal = light.ambient;
	
	// attentuation
	float distance = length(lightPosition - v_Position);
	// constant linear quadric
	float attentuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	
	diffuseFinal *= attentuation;
	ambientFinal *= attentuation;
	specularFinal *= attentuation;

	return texture(diffuse, v_TextCoord) * ambientFinal * 10;
}
vec3 calculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	return vec3(1.0, 1.0, 1.0);
}