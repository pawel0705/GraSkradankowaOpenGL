#version 430 

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

out vec3 v_Color;
out vec2 v_TextCoord;

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_textcoord;
layout(location = 3) in vec3 v_normal;

void main() {
  gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(v_position, 1.f);

  v_Color = v_color;
  v_TextCoord = v_textcoord;
}