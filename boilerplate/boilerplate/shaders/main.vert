#version 430 core

uniform mat4 modelView;
uniform mat4 ortho;

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 col;

//varying vec3 N;
//varying vec3 v;

out vec3 C;

void main(void) {
	C = col;

	//v = vec3(gl_ModelViewMatrix * gl_Vertex);
	//N = normalize(gl_NormalMatrix * gl_Normal);

	gl_Position = ortho * modelView * vec4(vertex, 1.0);
}
