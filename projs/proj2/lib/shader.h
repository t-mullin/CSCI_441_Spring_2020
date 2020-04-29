//
// Created by Tristan on 4/28/2020.
//

#ifndef PROJ2_SHADER_H
#define PROJ2_SHADER_H

#include <glm/glm.hpp>
#include "hit.h"
#include "light.h"
#include "ray.h"
#include "intersector.h"


class Shader {

public:

    glm::vec3 phongShader(const Hit& hit, const Light* light, const glm::vec3 eye, const World& world);
    glm::vec3 shadowShader();
};

glm::vec3 phongShader(const Hit &hit, const Light* light, const glm::vec3 eye, const World& world) {

    Intersector* _intersector;

    float ambient_coefficient = 0.2;
    float diffuse_coefficient = 1.0;
    float specular_coefficient = 0.5;
    float shinyness = 2.0;

    glm::vec3 pos = hit.position();

    glm::vec3 normal = hit.normal();    //n
    glm::vec3 light_dir = glm::normalize(light->direction(pos)); //l
    glm::vec3 view_dir = glm::normalize(eye - pos); //v

    float ambient = ambient_coefficient;
    glm::vec3 h = glm::normalize(-light_dir + (-view_dir)); //h
    float diffuse = diffuse_coefficient * glm::max(glm::dot(normal, light_dir), 0.0f);
    float specular = specular_coefficient * pow(glm::max(glm::dot(normal, h), 0.0f), shinyness);

    float e = 0.000003;
    Ray shadowRay;
    shadowRay.origin = e+pos;
    shadowRay.direction = light_dir;

    Hit shadowHit = _intersector->find_first_intersection(world, shadowRay);

    if(shadowHit.is_intersection()) {
        diffuse = 0.0f;
        specular = 0.0f;
    }

    glm::vec3 light_color = (ambient+diffuse+specular) * light->attenuation(pos) * light->color();

    return light_color*hit.color();
}


#endif //PROJ2_SHADER_H
