//
// Created by Tristan on 4/8/2020.
//

#ifndef LAB8_CAMERA_H
#define LAB8_CAMERA_H

#include <glm/glm.hpp>

class Camera {

public:
    glm::vec3 v_vec;
    glm::vec3 u_vec;
    glm::vec3 w_vec;

    Camera() {}

    ~Camera() {}

};

#endif //LAB8_CAMERA_H