#version 430

out vec4 out_Color;
in vec2 gs_TexCoord;

uniform sampler2D particleTexture;

void main()
{
	vec4 textured = texture(particleTexture, gs_TexCoord);
	
	if(textured.a < 0.6)
		discard;
	
	out_Color = textured;
}