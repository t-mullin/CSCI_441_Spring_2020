// Created by Tristan on 2/15/2020.
// CSCI 441 Spring 2020
// David Millman
// 2/18/2020

#ifndef LAB4_VECTOR3_H
#define LAB4_VECTOR3_H

#include <iostream>
#include <cmath>

class Vector3 {

public:
    float a[3];

    Vector3() {
        a[0] = 0;
        a[1] = 0;
        a[2] = 0;
    }

    Vector3(float array[3]) {
        a[0] = array[0];
        a[1] = array[1];
        a[2] = array[2];
    }

    Vector3(float x, float y, float z) {
        a[0] = x;
        a[1] = y;
        a[2] = z;
    }

    ~Vector3() {}

    //friends

    friend Vector3 crossProduct(Vector3 const v1, Vector3 const v2);
    friend Vector3 normalize(Vector3 const v);
    //operator overloads
    friend std::ostream& operator<<(std::ostream&, const Vector3& v);
    friend Vector3 operator-(Vector3 const v1, Vector3 const v2);
    friend Vector3 operator+(Vector3 const v1, Vector3 const v2);
};

//cross product of two vectors
Vector3 crossProduct(Vector3 const v1, Vector3 const v2) {
    float v[3];

    v[0] = (v1.a[1]*v2.a[2]) - (v1.a[2]*v2.a[1]);
    v[1] = (v1.a[2]*v2.a[0]) - (v1.a[0]*v2.a[2]);
    v[2] = (v1.a[0]*v2.a[1]) - (v1.a[1]*v2.a[0]);

    return Vector3(v);
}

//normalizes the input vector
Vector3 normalize(Vector3 const v) {
    float length = sqrtf(pow(v.a[0], 2) + pow(v.a[1], 2) + pow(v.a[2], 2));
    return Vector3((v.a[0]/length), (v.a[1]/length), (v.a[2]/length));
}

std::ostream &operator<<(std::ostream& stream, const Vector3 &v) {
    stream << v.a[0] << ", " <<  v.a[1] << ", " << v.a[2];
    return stream;
}

Vector3 operator-(Vector3 const v1, Vector3 const v2) {
    return Vector3((v1.a[0] - v2.a[0]), (v1.a[1] - v2.a[1]), (v1.a[2] - v2.a[2]));
}

Vector3 operator+(Vector3 const v1, Vector3 const v2) {
    return Vector3((v1.a[0] + v2.a[0]), (v1.a[1] + v2.a[1]), (v1.a[2] + v2.a[2]));
}

#endif //LAB4_VECTOR3_H
