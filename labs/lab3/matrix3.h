// Created by Tristan on 2/8/2020.
// CSCI 441 Spring 2020
// David Millman
// 2/11/2020

#ifndef LAB3_MATRIX3_H
#define LAB3_MATRIX3_H

class Matrix3 {

public:
    float a[9];
    //Constructor
    Matrix3() {
        //makes an identity matrix
        for(int i = 0; i < 9; i++) {
            if(i == 0 || i == 4 || i == 8) {
                a[i] = 1.0f;
            } else {
                a[i] = 0.0f;
            }
        }
    }

    //makes a new a matrix populated with a specific values
    Matrix3(float coor[9]) {
        for(int i = 0; i < 9; i++) {
            a[i] = coor[i];
        }
    }

    //Destructor
    ~Matrix3  () {

    }
    //friends
    friend Matrix3 operator* (float const s, Matrix3 const m);
    friend Matrix3 operator* (Matrix3 const m1, Matrix3 const m2);

};

//operator overload for scalar multiplication
Matrix3 operator* (float const s, Matrix3 const m){
    Matrix3 scaledMatrix;
    for(int i = 0; i < 9; i++) {
        scaledMatrix.a[i] = s*m.a[i];
    }
    return scaledMatrix;
}
//operator overload for matrix multiplication
Matrix3 operator*(Matrix3 const m1, Matrix3 const m2) {
    float b[9];
    b[0] = m1.a[0]*m2.a[0] + m1.a[1]*m2.a[3] + m1.a[2]*m2.a[6];
    b[1] = m1.a[0]*m2.a[1] + m1.a[1]*m2.a[4] + m1.a[2]*m2.a[7];
    b[2] = m1.a[0]*m2.a[2] + m1.a[1]*m2.a[5] + m1.a[2]*m2.a[8];

    b[3] = m1.a[3]*m2.a[0] + m1.a[4]*m2.a[3] + m1.a[5]*m2.a[6];
    b[4] = m1.a[3]*m2.a[1] + m1.a[4]*m2.a[4] + m1.a[5]*m2.a[7];
    b[5] = m1.a[3]*m2.a[2] + m1.a[4]*m2.a[5] + m1.a[5]*m2.a[8];

    b[6] = m1.a[6]*m2.a[0] + m1.a[7]*m2.a[3] + m1.a[8]*m2.a[6];
    b[7] = m1.a[6]*m2.a[1] + m1.a[7]*m2.a[4] + m1.a[8]*m2.a[7];
    b[8] = m1.a[6]*m2.a[2] + m1.a[7]*m2.a[5] + m1.a[8]*m2.a[8];
    return Matrix3(b);
}

#endif //LAB3_MATRIX3_H
