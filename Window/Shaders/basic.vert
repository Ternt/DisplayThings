#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

out vec3 vColor;

uniform mat4 modelMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;

void main() {
  gl_Position =  projMatrix * viewMatrix * modelMatrix * vec4(aPos, 1.0);
  vColor = aColor;
}
