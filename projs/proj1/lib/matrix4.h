//
// Created by Tristan on 3/26/2020.
//

#ifndef PROJ1_MATRIX4_H
#define PROJ1_MATRIX4_H

#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "vector4.h"

class Matrix4 {

public:
    float values[16];

    Matrix4() {
        //makes an identity matrix
        for(int i = 0; i < 16; i++) {
            if(i == 0 || i == 5 || i == 10 || i == 15) {
                values[i] = 1.0f;
            } else {
                values[i] = 0.0f;
            }
        }
    }

    //makes a new a matrix populated with a specific values
    Matrix4(float coor[16]) {
        for(int i = 0; i < 16; i++) {
            values[i] = coor[i];
        }
    }

    Matrix4(Vector4 v1, Vector4 v2, Vector4 v3) {
        Vector4 v4 = Vector4(0.0f, 0.0f, 0.0f);
        for(int i = 0; i < 16; i++) {
            if(i < 4) {
                values[i] = v1.values[i];
            } else if(i > 3 && i < 8) {
                values[i] = v2.values[i - 4];
            } else if(i > 7 && i < 12){
                values[i] = v3.values[i - 8];
            } else {
                values[i] = v4.values[i - 12];
            }
        }
    }
    //Destructor
    ~Matrix4() {}

    Matrix4 transpose(Matrix4 m);

    Matrix4 scale(float sx, float sy, float sz);
    Matrix4 scale(float s);

    Matrix4 rotate_x(int step);
    Matrix4 rotate_y(int step);
    Matrix4 rotate_z(int step);

    Matrix4 translate(float tx, float ty, float tz);

    Matrix4 viewport(int width, int height);
    Matrix4 orthographic(float l, float r, float b, float t, float n, float f);
    Matrix4 perspective(float l, float r, float b, float t, float n, float f);
    Matrix4 perspective(int width, int height, float FOV, float n, float f);

    //operator overloads
    friend std::ostream& operator<<(std::ostream& stream, const Matrix4& m);
    friend Matrix4 operator*(Matrix4 const m1, Matrix4 const m2);

};

Matrix4 transpose(Matrix4 m) {
    Matrix4 tpose;

    tpose.values[0] = m.values[0];
    tpose.values[1] = m.values[4];
    tpose.values[2] = m.values[8];
    tpose.values[3] = m.values[12];

    tpose.values[4] = m.values[1];
    tpose.values[5] = m.values[5];
    tpose.values[6] = m.values[9];
    tpose.values[7] = m.values[13];

    tpose.values[8] = m.values[2];
    tpose.values[9] = m.values[6];
    tpose.values[10] = m.values[10];
    tpose.values[11] = m.values[14];

    tpose.values[12] = m.values[3];
    tpose.values[13] = m.values[7];
    tpose.values[14] = m.values[11];
    tpose.values[15] = m.values[15];

    return tpose;
}

//builds a matrix for uneven scaling
Matrix4 scale(float sx, float sy, float sz) {
    Matrix4 scaleMatrix;
    scaleMatrix.values[0] = sx*scaleMatrix.values[0];
    scaleMatrix.values[5] = sy*scaleMatrix.values[5];
    scaleMatrix.values[10] = sz*scaleMatrix.values[10];
    return scaleMatrix;
}

//builds a matrix for even scaling
Matrix4 scale(float s) {
    Matrix4 scaleMatrix;
    scaleMatrix.values[0] = s*scaleMatrix.values[0];
    scaleMatrix.values[5] = s*scaleMatrix.values[5];
    scaleMatrix.values[10] = s*scaleMatrix.values[10];
    return scaleMatrix;
}

//builds a matrix for rotations about the x axis
Matrix4 rotate_x(int step) {
    Matrix4 rotX;
    float angleR = step * (M_PI/180.0f);
    rotX.values[5] = cosf(angleR);
    rotX.values[6] = -sinf(angleR);
    rotX.values[9] = sinf(angleR);
    rotX.values[10] = cosf(angleR);
    return transpose(rotX);
}

//builds a matrix for rotations about the y axis
Matrix4 rotate_y(int step) {
    Matrix4 rotY;
    float angleR = step * (M_PI/180.0f);
    rotY.values[0] = cosf(angleR);
    rotY.values[2] = sinf(angleR);
    rotY.values[8] = -sinf(angleR);
    rotY.values[10] = cosf(angleR);
    return transpose(rotY);
}

//builds a matrix for rotations about the z axis
Matrix4 rotate_z(int step) {
    Matrix4 rotZ;
    float angleR = step * (M_PI/180.0f);
    rotZ.values[0] = cosf(angleR);
    rotZ.values[1] = -sinf(angleR);
    rotZ.values[4] = sinf(angleR);
    rotZ.values[5] = cosf(angleR);
    return transpose(rotZ);
}

Matrix4 viewport(int width, int height) {
    float widthf = width;
    float heightf = height;
    Matrix4 viewport;

    viewport.values[0] = widthf/2.0f;
    viewport.values[3] = (widthf - 1.0f)/2.0f;
    viewport.values[5] = heightf/2.0f;
    viewport.values[7] = (heightf-1.0f)/2.0f;

    return transpose(viewport);
}

//builds a matrix for orthographic perspective
Matrix4 orthographic(float l, float r, float b, float t, float n, float f) {
    Matrix4 orthographic;

    orthographic.values[0] = 2.0f/(r-l);
    orthographic.values[3] = -(r+l)/(r-l);
    orthographic.values[5] = 2.0f/(t-b);
    orthographic.values[7] = -(t+b)/(t-b);
    orthographic.values[10] = -2.0f/(f-n);
    orthographic.values[11] = -(f+n)/(f-n);

    return transpose(orthographic);
}

//builds a matrix for projection
Matrix4 perspective(float l, float r, float b, float t, float n, float f) {
    Matrix4 perspective;

    perspective.values[0] = (2.0f*n)/(r-l);
    perspective.values[2] = (r+l)/(r-l);
    perspective.values[5] = (2.0f*n)/(t-b);
    perspective.values[6] = (t+b)/(t-b);
    perspective.values[10] = -(f+n)/(f-n);
    perspective.values[11] = -(2.0f*f*n)/(f-n);
    perspective.values[14] = -1;
    perspective.values[15] = 0;

    return transpose(perspective);
}

//builds a matrix for projection using FOV
Matrix4 perspective(int width, int height, float FOV, float n, float f) {
    Matrix4 perspective;
    float angleR = (FOV/2.0f) * (M_PI/180.0f);
    float aspectRatio = (float)width / (float)height;

    perspective.values[0] = 1.0f/(aspectRatio*tanf(angleR));
    perspective.values[5] = 1.0f/tanf(angleR);
    perspective.values[10] = -(f+n)/(f-n);
    perspective.values[11] = -(2.0f*f*n)/(f-n);
    perspective.values[14] = -1;
    perspective.values[15] = 0;

    return transpose(perspective);
}

//builds a matrix for translations
Matrix4 translate(float tx, float ty, float tz) {
    Matrix4 translation;
    translation.values[3] = tx;
    translation.values[7] = ty;
    translation.values[11] = tz;
    return transpose(translation);
}

Matrix4 operator*(Matrix4 const m1, Matrix4 const m2) {
    float temp[16];

    temp[0] = m1.values[0]*m2.values[0] + m1.values[1]*m2.values[4] + m1.values[2]*m2.values[8]  + m1.values[3]*m2.values[12];
    temp[1] = m1.values[0]*m2.values[1] + m1.values[1]*m2.values[5] + m1.values[2]*m2.values[9]  + m1.values[3]*m2.values[13];
    temp[2] = m1.values[0]*m2.values[2] + m1.values[1]*m2.values[6] + m1.values[2]*m2.values[10] + m1.values[3]*m2.values[14];
    temp[3] = m1.values[0]*m2.values[3] + m1.values[1]*m2.values[7] + m1.values[2]*m2.values[11] + m1.values[3]*m2.values[15];

    temp[4] = m1.values[4]*m2.values[0] + m1.values[5]*m2.values[4] + m1.values[6]*m2.values[8]  + m1.values[7]*m2.values[12];
    temp[5] = m1.values[4]*m2.values[1] + m1.values[5]*m2.values[5] + m1.values[6]*m2.values[9]  + m1.values[7]*m2.values[13];
    temp[6] = m1.values[4]*m2.values[2] + m1.values[5]*m2.values[6] + m1.values[6]*m2.values[10] + m1.values[7]*m2.values[14];
    temp[7] = m1.values[4]*m2.values[3] + m1.values[5]*m2.values[7] + m1.values[6]*m2.values[11] + m1.values[7]*m2.values[15];

    temp[8]  = m1.values[8]*m2.values[0] + m1.values[9]*m2.values[4] + m1.values[10]*m2.values[8]  + m1.values[11]*m2.values[12];
    temp[9]  = m1.values[8]*m2.values[1] + m1.values[9]*m2.values[5] + m1.values[10]*m2.values[9]  + m1.values[11]*m2.values[13];
    temp[10] = m1.values[8]*m2.values[2] + m1.values[9]*m2.values[6] + m1.values[10]*m2.values[10] + m1.values[11]*m2.values[14];
    temp[11] = m1.values[8]*m2.values[3] + m1.values[9]*m2.values[7] + m1.values[10]*m2.values[11] + m1.values[11]*m2.values[15];

    temp[12] = m1.values[12]*m2.values[0] + m1.values[13]*m2.values[4] + m1.values[14]*m2.values[8]  + m1.values[15]*m2.values[12];
    temp[13] = m1.values[12]*m2.values[1] + m1.values[13]*m2.values[5] + m1.values[14]*m2.values[9]  + m1.values[15]*m2.values[13];
    temp[14] = m1.values[12]*m2.values[2] + m1.values[13]*m2.values[6] + m1.values[14]*m2.values[10] + m1.values[15]*m2.values[14];
    temp[15] = m1.values[12]*m2.values[3] + m1.values[13]*m2.values[7] + m1.values[14]*m2.values[11] + m1.values[15]*m2.values[15];

    return Matrix4(temp);
}

std::ostream& operator<<(std::ostream& stream, const Matrix4& m) {
    for (int i = 0; i<16; i++) {
        if (i == 15) {
            stream << m.values[i];
        } else {
            if(i == 3 || i == 7 ||i == 11) {
                stream << m.values[i] << std::endl;
            } else {
                stream << m.values[i] << ", ";
            }
        }
    }
    return stream;
}

#endif //PROJ1_MATRIX4_H
