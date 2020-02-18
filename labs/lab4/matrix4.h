// Created by Tristan on 2/15/2020.
// CSCI 441 Spring 2020
// David Millman
// 2/18/2020

#ifndef LAB4_MATRIX4_H
#define LAB4_MATRIX4_H

#include <iostream>
#include <cmath>
#include "vector3.h"

class Matrix4 {

public:
    float a[16];
    //Constructor
    Matrix4() {
        //makes an identity matrix
        for(int i = 0; i < 16; i++) {
            if(i == 0 || i == 5 || i == 10 || i == 15) {
                a[i] = 1.0f;
            } else {
                a[i] = 0.0f;
            }
        }
    }

    //makes a new a matrix populated with a specific values
    Matrix4(float coor[16]) {
        for(int i = 0; i < 16; i++) {
            a[i] = coor[i];
        }
    }

    //Destructor
    ~Matrix4  () {

    }
    //friends
    friend Matrix4 buildScale4(float sx, float sy, float sz);
    friend Matrix4 buildScale4(float s);

    friend Matrix4 buildRotationX4(int step);
    friend Matrix4 buildRotationY4(int step);
    friend Matrix4 buildRotationZ4(int step);

    friend Matrix4 buildTranslation4(float tx, float ty, float tz);

    friend Matrix4 buildViewport4(int width, int height);
    friend Matrix4 buildOrthographic4(float l, float r, float b, float t, float n, float f);
    friend Matrix4 buildPerspective4(float l, float r, float b, float t, float n, float f);
    friend Matrix4 buildCamera4(Vector3 const gaze, Vector3 const up, Vector3 const eye);

    //operator overloads
    friend std::ostream& operator<<(std::ostream& stream, const Matrix4& m);
    friend Matrix4 operator*(Matrix4 const m1, Matrix4 const m2);
};

//builds a matrix for uneven scaling
Matrix4 buildScale4(float sx, float sy, float sz) {
    Matrix4 scaleMatrix;
    scaleMatrix.a[0] = sx*scaleMatrix.a[0];
    scaleMatrix.a[5] = sy*scaleMatrix.a[5];
    scaleMatrix.a[10] = sz*scaleMatrix.a[10];
    return scaleMatrix;
}

//builds a matrix for even scaling
Matrix4 buildScale4(float s) {
    Matrix4 scaleMatrix;
    scaleMatrix.a[0] = s*scaleMatrix.a[0];
    scaleMatrix.a[5] = s*scaleMatrix.a[5];
    scaleMatrix.a[10] = s*scaleMatrix.a[10];
    return scaleMatrix;
}

//builds a matrix for rotations about the x axis
Matrix4 buildRotationX4(int step) {
    const float PI = 3.14159265359;
    float angleR = step * (PI/20);
    float rotX[16] = {1, 0, 0, 0,
                      0, cosf(angleR), -sinf(angleR), 0,
                      0, sinf(angleR), cosf(angleR), 0,
                      0, 0, 0, 1};
    return Matrix4(rotX);
}

//builds a matrix for rotations about the y axis
Matrix4 buildRotationY4(int step) {
    const float PI = 3.14159265359;
    float angleR = step * (PI/20);
    float rotY[16] = {cosf(angleR), 0, sinf(angleR), 0,
                      0, 1, 0, 0,
                      -sinf(angleR), 0, cosf(angleR), 0,
                      0, 0, 0, 1};
    return Matrix4(rotY);
}

//builds a matrix for rotations about the z axis
Matrix4 buildRotationZ4(int step) {
    const float PI = 3.14159265359;
    float angleR = step * (PI/20);
    float rotZ[16] = {cosf(angleR), -sinf(angleR), 0, 0,
                      sinf(angleR), cosf(angleR), 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1};
    return Matrix4(rotZ);
}

//builds a matrix for translations
Matrix4 buildTranslation4(float tx, float ty, float tz) {
    Matrix4 translation;
    translation.a[3] = tx;
    translation.a[7] = ty;
    translation.a[11] = tz;
    return translation;
}

//builds a viewport matrix
Matrix4 buildViewport4(int width, int height) {
    float widthf = width;
    float heightf = height;
    float viewport[16];

    viewport[0] = widthf/2.0f;
    viewport[1] = 0;
    viewport[2] = 0;
    viewport[3] = (widthf - 1.0f)/2.0f;

    viewport[4] = 0;
    viewport[5] = heightf/2.0f;
    viewport[6] = 0;
    viewport[7] = (heightf-1.0f)/2.0f;

    viewport[8] = 0;
    viewport[9] = 0;
    viewport[10] = 1;
    viewport[11] = 0;

    viewport[12] = 0;
    viewport[13] = 0;
    viewport[14] = 0;
    viewport[15] = 1;

    return Matrix4(viewport);
}

//builds a matrix for orthographic perspective
Matrix4 buildOrthographic4(float l, float r, float b, float t, float n, float f) {
    float orthographic[16];

    orthographic[0] = 2.0f/(r-l);
    orthographic[1] = 0;
    orthographic[2] = 0;
    orthographic[3] = -(r+l)/(r-l);

    orthographic[4] = 0;
    orthographic[5] = 2.0f/(t-b);
    orthographic[6] = 0;
    orthographic[7] = -(t+b)/(t-b);

    orthographic[8] = 0;
    orthographic[9] = 0;
    orthographic[10] = -2.0f/(f-n);
    orthographic[11] = -(f+n)/(f-n);

    orthographic[12] = 0;
    orthographic[13] = 0;
    orthographic[14] = 0;
    orthographic[15] = 1;

    return Matrix4(orthographic);
}

//builds a matrix for projection
Matrix4 buildPerspective4(float l, float r, float b, float t, float n, float f) {
    float perspective[16];

    perspective[0] = (2.0f*n)/(r-l);
    perspective[1] = 0;
    perspective[2] = (r+l)/(r-l);
    perspective[3] = 0;

    perspective[4] = 0;
    perspective[5] = (2.0f*n)/(t-b);
    perspective[6] = (t+b)/(t-b);
    perspective[7] = 0;

    perspective[8] = 0;
    perspective[9] = 0;
    perspective[10] = -(f+n)/(f-n);
    perspective[11] = -(2.0f*f*n)/(f-n);

    perspective[12] = 0;
    perspective[13] = 0;
    perspective[14] = -1;
    perspective[15] = 0;

    return Matrix4(perspective);
}

//builds a matrix for projection using FOV
Matrix4 buildPerspective4(int width, int height, float FOV, float n, float f) {

    const float PI = 3.14159265359;
    float angleR = (FOV/2.0f) * (PI/180.0f);
    float aspectRatio = (float) width/ (float) height;

    float perspective[16];

    perspective[0] = 1.0f/(aspectRatio*tanf(angleR));
    perspective[1] = 0;
    perspective[2] = 0;
    perspective[3] = 0;

    perspective[4] = 0;
    perspective[5] = 1.0f/tanf(angleR);
    perspective[6] = 0;
    perspective[7] = 0;

    perspective[8] = 0;
    perspective[9] = 0;
    perspective[10] = -(f+n)/(f-n);
    perspective[11] = -(2.0f*f*n)/(f-n);

    perspective[12] = 0;
    perspective[13] = 0;
    perspective[14] = -1;
    perspective[15] = 0;

    return Matrix4(perspective);
}

//builds a matrix for the camera
Matrix4 buildCamera4(Vector3 const gaze, Vector3 const up, Vector3 const eye) {

    Vector3 w = normalize(eye-gaze);
    w.a[0] = -w.a[0];
    w.a[1] = -w.a[1];
    w.a[2] = -w.a[2];
    Vector3 u = normalize(crossProduct(up, w));
    Vector3 v = crossProduct(w, u);

    float uvwe[16];

    uvwe[0] = u.a[0];
    uvwe[1] = u.a[1];
    uvwe[2] = u.a[2];
    uvwe[3] = -eye.a[0];

    uvwe[4] = v.a[0];
    uvwe[5] = v.a[1];
    uvwe[6] = v.a[2];
    uvwe[7] = -eye.a[1];

    uvwe[8] = w.a[0];
    uvwe[9] = w.a[1];
    uvwe[10] = w.a[2];
    uvwe[11] = -eye.a[2];

    uvwe[12] = 0;
    uvwe[13] = 0;
    uvwe[14] = 0;
    uvwe[15] = 1.0;

    return Matrix4(uvwe);
}

std::ostream& operator<<(std::ostream& stream, const Matrix4& m) {
    for (int i = 0; i<16; i++) {
        if (i == 15) {
            stream << m.a[i];
        } else {
            if(i == 3 || i == 7 ||i == 11) {
                stream << m.a[i] << std::endl;
            } else {
                stream << m.a[i] << ", ";
            }
        }
    }
    return stream;
}

Matrix4 operator*(Matrix4 const m1, Matrix4 const m2) {
    float temp[16];

    temp[0] = m1.a[0]*m2.a[0] + m1.a[1]*m2.a[4] + m1.a[2]*m2.a[8]  + m1.a[3]*m2.a[12];
    temp[1] = m1.a[0]*m2.a[1] + m1.a[1]*m2.a[5] + m1.a[2]*m2.a[9]  + m1.a[3]*m2.a[13];
    temp[2] = m1.a[0]*m2.a[2] + m1.a[1]*m2.a[6] + m1.a[2]*m2.a[10] + m1.a[3]*m2.a[14];
    temp[3] = m1.a[0]*m2.a[3] + m1.a[1]*m2.a[7] + m1.a[2]*m2.a[11] + m1.a[3]*m2.a[15];

    temp[4] = m1.a[4]*m2.a[0] + m1.a[5]*m2.a[4] + m1.a[6]*m2.a[8]  + m1.a[7]*m2.a[12];
    temp[5] = m1.a[4]*m2.a[1] + m1.a[5]*m2.a[5] + m1.a[6]*m2.a[9]  + m1.a[7]*m2.a[13];
    temp[6] = m1.a[4]*m2.a[2] + m1.a[5]*m2.a[6] + m1.a[6]*m2.a[10] + m1.a[7]*m2.a[14];
    temp[7] = m1.a[4]*m2.a[3] + m1.a[5]*m2.a[7] + m1.a[6]*m2.a[11] + m1.a[7]*m2.a[15];

    temp[8]  = m1.a[8]*m2.a[0] + m1.a[9]*m2.a[4] + m1.a[10]*m2.a[8]  + m1.a[11]*m2.a[12];
    temp[9]  = m1.a[8]*m2.a[1] + m1.a[9]*m2.a[5] + m1.a[10]*m2.a[9]  + m1.a[11]*m2.a[13];
    temp[10] = m1.a[8]*m2.a[2] + m1.a[9]*m2.a[6] + m1.a[10]*m2.a[10] + m1.a[11]*m2.a[14];
    temp[11] = m1.a[8]*m2.a[3] + m1.a[9]*m2.a[7] + m1.a[10]*m2.a[11] + m1.a[11]*m2.a[15];

    temp[12] = m1.a[12]*m2.a[0] + m1.a[13]*m2.a[4] + m1.a[14]*m2.a[8]  + m1.a[15]*m2.a[12];
    temp[13] = m1.a[12]*m2.a[1] + m1.a[13]*m2.a[5] + m1.a[14]*m2.a[9]  + m1.a[15]*m2.a[13];
    temp[14] = m1.a[12]*m2.a[2] + m1.a[13]*m2.a[6] + m1.a[14]*m2.a[10] + m1.a[15]*m2.a[14];
    temp[15] = m1.a[12]*m2.a[3] + m1.a[13]*m2.a[7] + m1.a[14]*m2.a[11] + m1.a[15]*m2.a[15];

    return Matrix4(temp);
}

#endif //LAB4_MATRIX4_H
