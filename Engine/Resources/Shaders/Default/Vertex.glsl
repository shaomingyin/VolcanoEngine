#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

uniform mat4 volcanoModelViewMatrix;
uniform mat4 volcanoProjectionMatrix;
uniform mat4 volcanoNormalMatrix;

void main() {
    gl_Position = volcanoProjectionMatrix * volcanoModelViewMatrix * position;
}
