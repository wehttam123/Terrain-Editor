#version 430 core

uniform mat4 modelView;
uniform mat4 ortho;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 col;

out vec3 C;

void main(void) {
	C = col;
	gl_Position = ortho * modelView * vec4(vertex, 1.0);  
}
