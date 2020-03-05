//
// Created by Tristan on 2/29/2020.
//

#ifndef LAB6_TRIANGLE_H
#define LAB6_TRIANGLE_H

#include "vertex.h"

class Triangle {
public:
    int v[3];

    Triangle() {}

    Triangle(int v1, int v2, int v3) {
        v[0] = v1;
        v[1] = v2;
        v[2] = v3;
    }

    ~Triangle(){}


};

#endif //LAB6_TRIANGLE_H
