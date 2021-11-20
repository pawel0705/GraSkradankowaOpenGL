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
	if (gl_FrontFacing) {
		// ambient light
		vec3 ambientFinal = vec3(vec3(1.0f, 1.0f, 1.0f));
	
		// attentuation
		float distance = length(lightPosition - v_Position);
		// constant linear quadric
		float attentuation = 1.0f / (1.0f + 0.02f * distance + 0.065f * (distance * distance));

		ambientFinal *= attentuation;
	
		gl_FragColor = texture(diffuse, v_TextCoord) * (vec4(ambientFinal, 1.0f));
	}
}