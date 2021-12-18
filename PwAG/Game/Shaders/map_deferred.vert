#version 430 

uniform mat4 ModelMatrix;

out vec2 v_TextCoord;
out mat3 TBN;
out vec3 v_Offset;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec3 v_normal;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in vec2 v_textcoord;
layout(location = 6) in vec3 offset;

void main() {
	vec3 T = normalize(vec3(ModelMatrix * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(ModelMatrix * vec4(v_normal + offset, 0.0)));
	// re-orthogonalize T with respect to N
	T = normalize(T - dot(T, N) * N);
	// then retrieve perpendicular vector B with the cross product of T and N
	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);

	v_TextCoord = v_textcoord;  
	v_Offset = offset;

	gl_Position = vec4(v_position + offset, 1.0f);
}