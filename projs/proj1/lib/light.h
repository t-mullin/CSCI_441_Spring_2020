//
// Created by Tristan on 3/26/2020.
//

#ifndef PROJ1_LIGHT_H
#define PROJ1_LIGHT_H

#include "vector4.h"

class Light {
public:
    Vector4 lightPosition;
    Vector4 lightColor;
    float lightIntensity;

    Light(Vector4 lightPos, Vector4 lightCol, float lightInt) {
        lightPosition = lightPos;
        lightColor = lightCol;
        lightIntensity = lightInt;
    }
};
#endif //PROJ1_LIGHT_H
