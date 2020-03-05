//
// Created by Tristan on 2/29/2020.
//

#ifndef LAB6_VERTEX_H
#define LAB6_VERTEX_H

class Vertex {

public:
    float position[3];
    float color[3];
    float normal[3];
    float numOfDuplicates;

    Vertex(float x, float y, float z, float r, float g, float b, float nx, float ny, float nz) {
        position[0] = x;
        position[1] = y;
        position[2] = z;
        color[0] = r;
        color[1] = g;
        color[2] = b;
        normal[0] = nx;
        normal[1] = ny;
        normal[2] = nz;
        numOfDuplicates = 1;
    }

    ~Vertex() {}
};

#endif //LAB6_VERTEX_H
