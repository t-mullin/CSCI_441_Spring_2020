// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020

#ifndef PROJ1_VECTOR4_H
#define PROJ1_VECTOR4_H

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

class Vector4 {

public:

    float values[4];

    //for making a vector <0, 0, 0, 1>
    Vector4() {
        for(int i = 0; i < 3; i++) {
            values[i] = 0.0f;
        }
        values[3] = 1.0f;
    }

    //for making a vector using specific values of x, y, and z
    Vector4(float x, float y, float z) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = 1.0f;
    }

    //for making a vector using specific values of x, y, z, and w
    Vector4(float x, float y, float z, float w) {
        values[0] = x;
        values[1] = y;
        values[2] = z;
        values[3] = w;
    }

    //Destructor
    ~Vector4() {}

    Vector4 cross(Vector4 const v1, Vector4 const v2);
    Vector4 normalize(Vector4 const v);
    Vector4 rotate_y(int step, Vector4 const v);
    Vector4 rotate_z(int step, Vector4 const v);
    friend Vector4 operator+(Vector4 const v1, Vector4 const v2);
    friend Vector4 operator-(Vector4 const v1, Vector4 const v2);
    friend Vector4 operator*(float const scale, Vector4 const v);
    friend std::ostream& operator<<(std::ostream& stream, const Vector4& v);

};

//takes the cross product of 2 vectors
Vector4 cross(Vector4 const v1, Vector4 const v2) {
    Vector4 v;

    v.values[0] = (v1.values[1]*v2.values[2]) - (v1.values[2]*v2.values[1]);
    v.values[1] = (v1.values[2]*v2.values[0]) - (v1.values[0]*v2.values[2]);
    v.values[2] = (v1.values[0]*v2.values[1]) - (v1.values[1]*v2.values[0]);
    v.values[3] = 0;

    return v;
}

//normalizes a vector
Vector4 normalize(Vector4 const v) {
    float length = sqrtf(powf(v.values[0], 2) + powf(v.values[1], 2) + powf(v.values[2], 2) + powf(v.values[3], 2));
    return Vector4((v.values[0]/length), (v.values[1]/length), (v.values[2]/length), (v.values[3]/length));
}

//rotates the vector around the y coordinate
Vector4 rotate_y(int step, Vector4 const v) {
    float angleR = step * (M_PI/180.0f);
    return Vector4(v.values[0]*cosf(angleR), v.values[1],v.values[2]*-sinf(angleR), 0);
}

//rotates the vector around the z coordinate
Vector4 rotate_z(int step, Vector4 const v) {
    float angleR = step * (M_PI/180.0f);
    return Vector4(v.values[0], v.values[1]*cosf(angleR),v.values[2], 0);
}

// + operator overload for vector addition
Vector4 operator+(Vector4 const v1, Vector4 const v2) {
    return Vector4((v1.values[0] + v2.values[0]), (v1.values[1] + v2.values[1]), (v1.values[2] + v2.values[2]), (v1.values[3] + v2.values[3]));
}

// - operator overload for vector subtraction
Vector4 operator-(Vector4 const v1, Vector4 const v2) {
    return Vector4((v1.values[0] - v2.values[0]), (v1.values[1] - v2.values[1]), (v1.values[2] - v2.values[2]), (v1.values[3] - v2.values[3]));
}

// * operator overload for vector scaling
Vector4 operator*(float const scale, Vector4 const v) {
    return Vector4(scale*v.values[0],scale*v.values[1],scale*v.values[2],scale*v.values[3]);
}

// << operator overload for printing out the vector to the terminal
std::ostream& operator<<(std::ostream& stream, const Vector4& v) {
    stream << "<" << v.values[0] << ", " <<  v.values[1] << ", " << v.values[2] << ", " << v.values[3] << ">";
    return stream;
}

#endif //PROJ1_VECTOR4_H
