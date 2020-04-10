// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/14/2020

#ifndef LAB8_CAMERA_H
#define LAB8_CAMERA_H

#include <glm/glm.hpp>

class Camera {

public:
    glm::vec3 v_vec;
    glm::vec3 u_vec;
    glm::vec3 w_vec;

    Camera(glm::vec3 eye, glm::vec3 origin, glm::vec3 up) {
            w_vec = -1.0f*glm::normalize(origin-eye);
            u_vec = glm::normalize(glm::cross(up, w_vec));
            v_vec = glm::cross(w_vec, u_vec);
    }

    ~Camera() {}

};

#endif //LAB8_CAMERA_H
