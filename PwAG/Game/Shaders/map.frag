#version 430 

uniform sampler2D diffuse;

uniform vec3 ambientLight;

in vec3 v_Color;
in vec2 v_TextCoord;
in vec3 v_Normal;

void main()
{
	if (gl_FrontFacing) {
		gl_FragColor = texture(diffuse, v_TextCoord) * vec4(ambientLight, 1.0f);
	}
  // gl_FragColor = vec4(v_Color, 1.0f);
}