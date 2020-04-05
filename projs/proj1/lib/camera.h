// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020
/*
 * Reused and modified the Camera class provided by David Millman
 */

#ifndef PROJ1_CAMERA_H
#define PROJ1_CAMERA_H

#include "matrix4.h"
#include "vector4.h"

class Camera {

public:

    Matrix4 orthogonal;
    Matrix4 perspective;
    Matrix4 lookAt;
    Vector4 eye;
    Vector4 origin;
    Vector4 up;

    //default camera constructor
    Camera() {
        eye = Vector4(0, 0,0);
        origin = Vector4(0, 0,0);
        up = Vector4(0, 0,0);
        lookAt = Matrix4(eye,origin,up);
    }

    //build a camera with specific values for eye, origin, and up vectors
    Camera(Vector4 e, Vector4 o, Vector4 u)  {
        eye = e;
        origin = o;
        up = u;
        lookAt = Matrix4(eye,origin,up);
    }

    //Destructor
    ~Camera() {}

    Matrix4 look_at();

};

//creates a lookAt matrix
Matrix4 Camera::look_at() {
    Matrix4 camera;
    Matrix4 gazeMat;

    Vector4 gaze = origin-eye;
    Vector4 w = normalize(gaze);
    w.values[0] = -w.values[0];
    w.values[1] = -w.values[1];
    w.values[2] = -w.values[2];
    w.values[3] = 1.0f;
    Vector4 u = normalize(cross(up, w));
    Vector4 v = cross(w, u);

    camera.values[0] = u.values[0];
    camera.values[1] = u.values[1];
    camera.values[2] = u.values[2];

    camera.values[4] = v.values[0];
    camera.values[5] = v.values[1];
    camera.values[6] = v.values[2];

    camera.values[8] = w.values[0];
    camera.values[9] = w.values[1];
    camera.values[10] = w.values[2];

    gazeMat.values[3] = -eye.values[0];
    gazeMat.values[7] = -eye.values[1];
    gazeMat.values[11] = -eye.values[2];

    return transpose(camera*gazeMat);
}

#endif //PROJ1_CAMERA_H
