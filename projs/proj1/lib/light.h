// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020

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

    ~Light() {}
};
#endif //PROJ1_LIGHT_H
