//
// Created by Tristan on 2/29/2020.
//

#ifndef LAB6_MESH_H
#define LAB6_MESH_H

#include <vector>
#include "vertex.h"
#include "triangle.h"

class Mesh {

public:
    std::vector<Vertex> verts;
    std::vector<Triangle> triangles; //these need to be the indeces to verts

    Mesh(std::vector<float> coords) {
        addVertices(coords);
        generateTriangles(coords);
        generateSmoothNormals();
    }

    ~Mesh() {

    }

    void addVertices(std::vector<float> coords) {
        for(int i = 0; i < coords.size(); i+=9) {
            verts.push_back(Vertex(coords.at(i), coords.at(i + 1), coords.at(i + 2),
                                   coords.at(i + 3), coords.at(i + 4), coords.at(i + 5),
                                   coords.at(i + 6), coords.at(i + 7), coords.at(i + 8)));
        }
        removeDuplicates();
    }

    void removeDuplicates() {
        for (int i = 0; i < verts.size(); i++) {
            for(int j = i+1; j< verts.size(); j++) {
                if(verts[i].position[0] == verts[j].position[0] &&
                    verts[i].position[1] == verts[j].position[1] &&
                    verts[i].position[2] == verts[j].position[2]) {
                    addNormals(i, j);
                    verts.erase(verts.begin()+j);
                }
            }
        }
    }

    void addNormals(int i, int j) {
        verts[i].normal[0] += verts[j].normal[0];
        verts[i].normal[1] += verts[j].normal[1];
        verts[i].normal[2] += verts[j].normal[2];
        verts[i].numOfDuplicates++;
    }

    void generateSmoothNormals() {
        for(int i = 0; i < verts.size(); i++) {
            verts[i].normal[0] = verts[i].normal[0]/verts[i].numOfDuplicates;
            verts[i].normal[1] = verts[i].normal[1]/verts[i].numOfDuplicates;
            verts[i].normal[2] = verts[i].normal[2]/verts[i].numOfDuplicates;
        }
    }

    void generateTriangles(std::vector<float> coords) {
        int v1 = 0;
        int v2 = 0;
        int v3 = 0;
        for(int i = 0; i < coords.size(); i+=27) {
            for (int j = 0; j < verts.size(); j++) {
                if(coords[i] == verts[j].position[0] && coords[i+1] == verts[j].position[1] && coords[i+2] == verts[j].position[2] ) {
                   v1 = j;
                }
                if(coords[i + 9] == verts[j].position[0] && coords[i+10] == verts[j].position[1] && coords[i+11] == verts[j].position[2] ) {
                    v2 = j;
                }
                if(coords[i+18] == verts[j].position[0] && coords[i+19] == verts[j].position[1] && coords[i+20] == verts[j].position[2] ) {
                    v3 = j;
                }
            }
            triangles.push_back(Triangle(v1, v2, v3));
        }
    }
};


#endif //LAB6_MESH_H
