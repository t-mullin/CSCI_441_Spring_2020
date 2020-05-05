// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 5/6/2020

//Code provided by David Millman

#ifndef _CSCI441_RAY_H_
#define _CSCI441_RAY_H_

#include <glm/glm.hpp>

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 eval(float t) const {
        return origin + t*direction;
    }
};

#endif
