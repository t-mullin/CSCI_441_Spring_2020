// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/14/2020

#ifndef LAB8_RAY_H
#define LAB8_RAY_H

#include <glm/glm.hpp>

class Ray {

public:
    glm::vec3 origin;
    glm::vec3 direction;

    Ray() {}

    ~Ray() {}

};

#endif //LAB8_RAY_H
