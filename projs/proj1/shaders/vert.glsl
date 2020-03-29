#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

out vec3 ourPos;
out vec3 ourNormal;
out vec2 ourTex;

void main() {
    gl_Position = projection * camera * model * vec4(aPos, 1.0);
    ourNormal = aNormal;
    ourPos = vec3(model * vec4(aPos, 1.0));
    ourTex = aTex;
}
