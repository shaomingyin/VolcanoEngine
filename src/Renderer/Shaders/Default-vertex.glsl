#version 330

layout (location = 0) in vec3 position;

uniform mat4 projectMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main(void)
{
    gl_Position = projectMatrix * viewMatrix * modelMatrix * position;
}
