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

    //creates a light object with specific values for position, color, and intensity
    Light(Vector4 lightPos, Vector4 lightCol, float lightInt) {
        lightPosition = lightPos;
        lightColor = lightCol;
        lightIntensity = lightInt;
    }

    //Destructor
    ~Light() {}

};
#endif //PROJ1_LIGHT_H
