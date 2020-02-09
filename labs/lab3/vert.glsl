#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

out vec3 myColor;

uniform mat3 transform;

void main() {
    vec3 position = vec3(aPos, 1.0);
    gl_Position = vec4(transform*position, 1.0);
    myColor = aColor;
}
