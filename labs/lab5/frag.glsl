#version 330 core
in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourPos;

out vec4 fragColor;

uniform vec3 lightSource;
uniform vec3 lightColor;
uniform float lightIntensity;
uniform vec3 eye;

vec3 light = lightIntensity * lightColor;

vec3 lightDir = normalize(lightSource - ourPos);
float diff = max(dot(ourNormal, lightDir), 0.0f);
vec3 diffuse = diff * light;

float specularMod = 0.5f;
vec3 viewDir = normalize(eye - ourPos);
vec3 reflectDir = reflect(-lightDir, ourNormal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
vec3 specular = specularMod * spec * light;

void main() {
    float ambientMod = 0.1f;
    vec3 ambient = ambientMod * light;

    fragColor = vec4((ambient + diffuse + specular)*ourColor, 1.0f);
}
