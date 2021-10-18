#version 430 

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
out vec3 v_Color;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec3 vertex_normal;

void main() {
  gl_Position =  ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);

  v_Color = vertex_color;
}