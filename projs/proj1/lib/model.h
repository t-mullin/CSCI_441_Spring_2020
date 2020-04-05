// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020

#ifndef PROJ1_MODEL_H
#define PROJ1_MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <csci441/shader.h>

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>

#include "matrix4.h"
#include "camera.h"

#define LINE_SIZE 256

//struct to hold vertex info
struct ModelVertex {
    float position[3];
    float normal[3];
    float uv[2];
};

//stuct to hold triangle info
struct Triangle {
    unsigned int TriangleIndices[3];
};

class Model {

public:

    std::vector<ModelVertex> vertices;
    std::vector<Triangle> triangles;
    std::vector<std::vector<float>> UV;
    std::vector<std::vector<float>> normals;
    std::vector<unsigned int> indices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;
    int textureIndex;
    Matrix4 model;
    Shader shader;
    GLuint VAO, VBO, EBO;

    //creates a model object with a texture
    Model(const char *modelPath, const std::string& texturePath, int texIndex, const Shader& modelShader, bool hasUV, bool hasNormals) : shader(modelShader) {
        loadOBJ(modelPath, hasUV, hasNormals);
        textureIndex = texIndex;
        setupVAO();
        setupTextures(texturePath);
    }

    //creates a model object that does not have a texture
    Model(const char *modelPath, const Shader& modelShader, bool hasUV, bool hasNormals) : shader(modelShader) {
        loadOBJ(modelPath, hasUV, hasNormals);
        setupVAO();
    }

    //Destructor
    ~Model() {}

    void setupVAO();
    void setupTextures(const std::string& path);

private:

    void loadOBJ(const char *path, bool hasUV, bool hasNormals);
    void setUV();
    void setNormals();
    void calcVertexNorm();
    GLuint createTexture();
    GLuint loadTexture(const std::string& path);

};

//loads and parses an object file
void Model::loadOBJ(const char *path, bool hasUV, bool hasNormals) {
    FILE *inp = fopen(path, "r");
    if(inp == NULL) {
        std::cout << "Object failed to load at path: " << path << std::endl;
    }
    while(!feof(inp)) {
        char line[LINE_SIZE];
        fscanf(inp, "%s", line);
        ModelVertex vertex;
        if(strcmp(line, "v") == 0) {
            fscanf(inp, "%f %f %f\n", &vertex.position[0], &vertex.position[1], &vertex.position[2]);
            vertex.normal[0] = 0;
            vertex.normal[1] = 0;
            vertex.normal[2] = 0;
            vertices.push_back(vertex);
        } else if(strcmp(line, "vt") == 0) {
            std::vector<float> uv;
            float u;
            float v;
            fscanf(inp, "%f %f\n", &u, &v);
            uv.push_back(u);
            uv.push_back(v);
            UV.push_back(uv);
        } else if(strcmp(line, "vn") == 0) {
            float normal[3];
            std::vector<float> tempNorm;
            fscanf(inp, "%f %f %f\n", &normal[0], &normal[1], &normal[2]);
            tempNorm.push_back(normal[0]);
            tempNorm.push_back(normal[1]);
            tempNorm.push_back(normal[2]);
            normals.push_back(tempNorm);
        } else if(strcmp(line, "f") == 0) {
            unsigned int index[3];
            unsigned int uvIndex[3];
            unsigned int normalIndex[3];
            Triangle triangle;
            if (!hasUV && !hasNormals) {
                fscanf(inp, "%d %d %d\n", &index[0], &index[1], &index[2]);
                triangle.TriangleIndices[0] = index[0];
                triangle.TriangleIndices[1] = index[1];
                triangle.TriangleIndices[2] = index[2];
                triangles.push_back(triangle);
                indices.push_back(index[0]-1);
                indices.push_back(index[1]-1);
                indices.push_back(index[2]-1);
            } else if(hasUV && hasNormals) {
                fscanf(inp, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &index[0], &uvIndex[0], &normalIndex[0], &index[1], &uvIndex[1], &normalIndex[1], &index[2], &uvIndex[2], &normalIndex[2]);
                triangle.TriangleIndices[0] = index[0];
                triangle.TriangleIndices[1] = index[1];
                triangle.TriangleIndices[2] = index[2];
                triangles.push_back(triangle);
                for(int i = 0; i < 3; i++) {
                    indices.push_back(index[i]-1);
                    uvIndices.push_back(uvIndex[i]-1);
                    normalIndices.push_back(normalIndex[i]-1);
                }
            }
        }
    }
    fclose(inp);
    if(hasUV) {
        setUV();
    }
    if(!hasNormals) {
        calcVertexNorm();
    } else {
        setNormals();
    }
}

//sets the models UV data loaded from obj file
void Model::setUV() {
    for(int i = 0; i < indices.size(); i++) {
        vertices[indices[i]].uv[0] = UV[uvIndices[i]][0];
        vertices[indices[i]].uv[1] = UV[uvIndices[i]][1];
    }
}

//sets the models Normal data loaded from obj file
void Model::setNormals() {
    for(int i = 0; i < indices.size(); i++) {
        if(normals.size() == 1) {
            vertices[indices[i]].normal[0] = normals[0][0];
            vertices[indices[i]].normal[1] = normals[0][1];
            vertices[indices[i]].normal[2] = normals[0][2];
        }
        vertices[indices[i]].normal[0] = normals[normalIndices[i]][0];
        vertices[indices[i]].normal[1] = normals[normalIndices[i]][1];
        vertices[indices[i]].normal[2] = normals[normalIndices[i]][2];
    }
}

//manually calculates normals based on vertex data
void Model::calcVertexNorm() {
    for(int i = 0; i < triangles.size(); i++) {
        unsigned int vertIndex1 = triangles.at(i).TriangleIndices[0] - 1;
        unsigned int vertIndex2 = triangles.at(i).TriangleIndices[1] - 1;
        unsigned int vertIndex3 = triangles.at(i).TriangleIndices[2] - 1;

        float a[3];
        float b[3];
        float c[3];

        float length;

        a[0] = vertices.at(vertIndex2).position[0] - vertices.at(vertIndex1).position[0];
        a[1] = vertices.at(vertIndex2).position[1] - vertices.at(vertIndex1).position[1];
        a[2] = vertices.at(vertIndex2).position[2] - vertices.at(vertIndex1).position[2];

        b[0] = vertices.at(vertIndex3).position[0] - vertices.at(vertIndex1).position[0];
        b[1] = vertices.at(vertIndex3).position[1] - vertices.at(vertIndex1).position[1];
        b[2] = vertices.at(vertIndex3).position[2] - vertices.at(vertIndex1).position[2];

        c[0] = (a[1]*b[2]) - (a[2]*b[1]);
        c[1] = (a[2]*b[0]) - (a[0]*b[2]);
        c[2] = (a[0]*b[1]) - (a[1]*b[0]);

        length = sqrtf(pow(c[0],2) + pow(c[1],2) + pow(c[2],2));

        vertices.at(vertIndex1).normal[0] += c[0]/length;
        vertices.at(vertIndex1).normal[1] += c[1]/length;
        vertices.at(vertIndex1).normal[2] += c[2]/length;

        vertices.at(vertIndex2).normal[0] += c[0]/length;
        vertices.at(vertIndex2).normal[1] += c[1]/length;
        vertices.at(vertIndex2).normal[2] += c[2]/length;

        vertices.at(vertIndex3).normal[0] += c[0]/length;
        vertices.at(vertIndex3).normal[1] += c[1]/length;
        vertices.at(vertIndex3).normal[2] += c[2]/length;

    }

    for(int i = 0; i < vertices.size(); i++) {
        vertices.at(i).normal[0] = vertices.at(i).normal[0]/6.0f;
        vertices.at(i).normal[1] = vertices.at(i).normal[1]/6.0f;
        vertices.at(i).normal[2] = vertices.at(i).normal[2]/6.0f;
    }
}

//creates checkerboard pattern
GLuint Model::createTexture() {
    GLuint textureID;
    const int WIDTH = 250; //width of texture
    const int HEIGHT = 250; //height of texture
    const int CELLSIZE = 10; //sets the number of cells per row
    int position = 0; //keeps track of the current position in the 1D array
    GLuint texture[WIDTH*HEIGHT]; //array to hold texture

    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            //checks what row we are on
            if(i < HEIGHT/CELLSIZE || (i > (2*HEIGHT)/CELLSIZE && i < (3*HEIGHT)/CELLSIZE) || (i > (4*HEIGHT)/CELLSIZE && i < (5*HEIGHT)/CELLSIZE) || (i > (6*HEIGHT)/CELLSIZE && i < (7*HEIGHT)/CELLSIZE) || (i > (8*HEIGHT)/CELLSIZE && i < (9*HEIGHT)/CELLSIZE)) {
                //checks if we are in a certain range and picks either white or black
                if(j < WIDTH/CELLSIZE || (j > (2*WIDTH)/CELLSIZE && j < (3*WIDTH)/CELLSIZE) || (j > (4*WIDTH)/CELLSIZE && j < (5*WIDTH)/CELLSIZE) || (j > (6*WIDTH)/CELLSIZE && j < (7*WIDTH)/CELLSIZE) || (j > (8*WIDTH)/CELLSIZE && j < (9*WIDTH)/CELLSIZE)) {
                    texture[position] = 0xFFFFFFFF;
                    position++;
                } else {
                    texture[position] = 0x00000000;
                    position++;
                }
            } else {
                //checks if we are in a certain range and picks either white or black
                if(j < WIDTH/CELLSIZE || (j > (2*WIDTH)/CELLSIZE && j < (3*WIDTH)/CELLSIZE) || (j > (4*WIDTH)/CELLSIZE && j < (5*WIDTH)/CELLSIZE) || (j > (6*WIDTH)/CELLSIZE && j < (7*WIDTH)/CELLSIZE) || (j > (8*WIDTH)/CELLSIZE && j < (9*WIDTH)/CELLSIZE)) {
                    texture[position] = 0x00000000;
                    position++;
                } else {
                    texture[position] = 0xFFFFFFFF;
                    position++;
                }
            }
        }
    }

    //generating the texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return textureID;
}

//loads texture from a image file
GLuint Model::loadTexture(const std::string& path) {
    bool flip=true;
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(flip);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = 0;
        switch (nrComponents) {
            case 1: format = GL_RED; break;
            case 3: format = GL_RGB; break;
            case 4: format = GL_RGBA; break;
        }

        //generating the texture and mipmap based on image input
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

//sets up the VBO, EBO, and VAO
//Used code from https://learnopengl.com/Model-Loading/Mesh to troubleshoot why EBO data
//was not being loaded correctly. learned that the VAO has to be bound before EBO can be initialized
void Model::setupVAO() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(ModelVertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // setup position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(ModelVertex), (void*)offsetof(ModelVertex, uv));
}

//loads the texture and binds it to a specific index
void Model::setupTextures(const std::string& path) {
    if(textureIndex == 0) {
        glActiveTexture(GL_TEXTURE0);
        GLuint texture = loadTexture(path);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else if(textureIndex == 1){
        glActiveTexture(GL_TEXTURE1);
        GLuint texture = loadTexture(path);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else if(textureIndex == 2) {
        glActiveTexture(GL_TEXTURE2);
        GLuint texture = loadTexture(path);
        glBindTexture(GL_TEXTURE_2D, texture);
    } else if(textureIndex == 3) {
        glActiveTexture(GL_TEXTURE3);
        GLuint texture = loadTexture(path);
        glBindTexture(GL_TEXTURE_2D, texture);
    }
}

#endif //PROJ1_MODEL_H
