#version 430 

uniform sampler2D diffuse;

in vec3 v_Color;
in vec2 v_TextCoord;

void main()
{
	
	gl_FragColor = texture(diffuse, v_TextCoord);
  // gl_FragColor = vec4(v_Color, 1.0f);
}