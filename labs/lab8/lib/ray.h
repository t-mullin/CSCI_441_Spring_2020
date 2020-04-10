//
// Created by Tristan on 4/7/2020.
//

#ifndef LAB8_RAY_H
#define LAB8_RAY_H

#include <glm/glm.hpp>

class Ray {

public:
    glm::vec3 origin;
    glm::vec3 direction;
    float magnitude;
    
    Ray() {}

    ~Ray() {}

};

#endif //LAB8_RAY_H
