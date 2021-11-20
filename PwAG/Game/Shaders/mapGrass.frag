#version 430 

uniform sampler2D diffuse;

uniform vec3 ambientLight;
uniform vec3 lightPosition;
uniform vec3 cameraPos;

in vec3 v_Color;
in vec2 v_TextCoord;
in vec3 v_Normal;
in vec3 v_Position;

void main()
{
		// diffuse light
		vec3 posToLightDirectionVector = normalize(lightPosition - v_Position);
		vec3 diffuseColor = vec3(1.0f, 1.0f, 1.0f);
		//float diffuseLight = clamp(dot(posToLightDirectionVector, normalize(v_Normal)), 0, 1);
		float diffuseLight = max(dot(normalize(v_Normal), posToLightDirectionVector), 0.0f);
		vec3 diffuseFinal = diffuseColor * diffuseLight;
	
		// specular light
		vec3 lightToPosDirectionVector = normalize(v_Position - lightPosition);
		vec3 reflectDirVec = normalize(reflect(lightToPosDirectionVector, normalize(v_Normal)));
		vec3 posToViewDirVec = normalize(cameraPos - v_Position);
		float specularConstant = pow(max(dot(posToViewDirVec, reflectDirVec), 0), 105);
		vec3 specularFinal = vec3(1.0f, 1.0f, 1.0f) * specularConstant * texture(diffuse, v_TextCoord).rgb;
	
		// ambient light
		vec3 ambientFinal = vec3(ambientLight);
	
		// attentuation
		float distance = length(lightPosition - v_Position);
		// constant linear quadric
		float attentuation = 1.0f / (1.0f + 0.02f * distance + 0.025f * (distance * distance));
	
		diffuseFinal *= attentuation;
		ambientFinal *= attentuation;
		specularFinal *= attentuation;
	
		if (texture(diffuse, v_TextCoord).a < 0.1)
			discard;
	
		gl_FragColor = texture(diffuse, v_TextCoord) * ((vec4(ambientFinal, 1.0f) + vec4(diffuseFinal, 1.0f) + vec4(specularFinal, 1.f)));
}