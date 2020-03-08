#version 330 core
in vec2 ourTex;

out vec4 fragColor;

uniform sampler2D ourTexture;
/**
 * TODO: PART-1 update the fragment shader to get the texture coordinates from
 * the vertex shader
 */
/**
 * TODO: PART-3 update the fragment shader to get the fragColor color from the
 * texture, and add the sampler2D.
 */
void main() {
    //fragColor = vec4(ourTex.x, ourTex.y, 0, 1);
    fragColor = texture(ourTexture, ourTex);
}
