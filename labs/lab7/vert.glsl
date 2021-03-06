#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 camera;

out vec2 ourTex;
/**
 * TODO: PART-1 update the vertex shader to take the texture coordinates
 * as input and send them to the fradgment shader.
 */
void main() {
    gl_Position = projection * camera * model * vec4(aPos, 1.0);
    ourTex = aTex;
}
