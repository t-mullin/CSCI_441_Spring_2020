// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 3/10/2020

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <csci441/shader.h>
#include <csci441/matrix4.h>
#include <csci441/matrix3.h>
#include <csci441/vector4.h>
#include <csci441/uniform.h>

#include "shape.h"
#include "camera.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
int spaceCount = 0; //keeps track of # of times space has been pressed

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool isPressed(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool isReleased(GLFWwindow *window, int key) {
    return glfwGetKey(window, key) == GLFW_RELEASE;
}

Matrix4 processModel(const Matrix4& model, GLFWwindow *window) {
    Matrix4 trans;

    const float ROT = 1;
    const float SCALE = .05;
    const float TRANS = .01;

    // ROTATE
    if (isPressed(window, GLFW_KEY_U)) { trans.rotate_x(-ROT); }
    else if (isPressed(window, GLFW_KEY_I)) { trans.rotate_x(ROT); }
    else if (isPressed(window, GLFW_KEY_O)) { trans.rotate_y(-ROT); }
    else if (isPressed(window, GLFW_KEY_P)) { trans.rotate_y(ROT); }
    else if (isPressed(window, '[')) { trans.rotate_z(-ROT); }
    else if (isPressed(window, ']')) { trans.rotate_z(ROT); }
    // SCALE
    else if (isPressed(window, '-')) { trans.scale(1-SCALE, 1-SCALE, 1-SCALE); }
    else if (isPressed(window, '=')) { trans.scale(1+SCALE, 1+SCALE, 1+SCALE); }
    // TRANSLATE
    else if (isPressed(window, GLFW_KEY_UP)) { trans.translate(0, TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_DOWN)) { trans.translate(0, -TRANS, 0); }
    else if (isPressed(window, GLFW_KEY_LEFT)) { trans.translate(-TRANS, 0, 0); }
    else if (isPressed(window, GLFW_KEY_RIGHT)) { trans.translate(TRANS, 0, 0); }
    else if (isPressed(window, ',')) { trans.translate(0,0,TRANS); }
    else if (isPressed(window, '.')) { trans.translate(0,0,-TRANS); }

    return trans * model;
}

bool isSpaceEvent(GLFWwindow *window) {
    static bool pressed = false;

    bool trigger = false;
    if (isPressed(window, GLFW_KEY_SPACE)) {
        pressed = true;
    } else if (pressed && isReleased(window, GLFW_KEY_SPACE)) {
        pressed = false;
        trigger = true;
    }
    return trigger;
}

void processInput(Matrix4& model, GLFWwindow *window) {
    if (isPressed(window, GLFW_KEY_ESCAPE) || isPressed(window, GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true);
    } else if (isSpaceEvent(window)) {
        /**
         * TODO: PART-6 for demo, add code here to change the mode without
         * having massive flickering
         **/
         spaceCount++; //increments each time space is pressed
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

GLuint createTexture() {
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
    /**
     * TODO: Part-2 create the checker texture
     */
     //generating the texture
     glGenTextures(1, &textureID);
     glBindTexture(GL_TEXTURE_2D, textureID);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, texture);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

GLuint loadTexture(const std::string& path, bool flip=true) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

        /**
         * TODO: Part-3 create a texture map for an image
         */
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

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CSCI441-lab", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // tell glfw what to do on resize
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // init glad
    if (!gladLoadGL()) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    // setup camera
    Matrix4 projection;
    projection.perspective(45, 1, .01, 10);

    Camera camera;
    camera.projection = projection;
    camera.eye = Vector4(0, 0, 3);
    camera.origin = Vector4(0, 0, 0);
    camera.up = Vector4(0, 1, 0);

    // setup the shader
    Shader shader("../vert.glsl", "../frag.glsl");

    /* init the shape */
    Sphere shape(20, 1);
    Cube shape2;
    // copy vertex data
    GLuint vbo, vbo2;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, shape.size(), shape.data(), GL_STATIC_DRAW);

    // describe vertex layout
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // setup position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    glBufferData(GL_ARRAY_BUFFER, shape2.size(), shape2.data(), GL_STATIC_DRAW);

    GLuint vao2;
    glGenVertexArrays(1, &vao2);
    glBindVertexArray(vao2);

    // setup position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    /** TODO: Part1 add vertex attribute pointer for texture coordinates */

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    // init the model matrix
    Matrix4 model;

    // setup the textures
    /** TODO: Part2 create and bind the texture. */

    glActiveTexture(GL_TEXTURE0);
    GLuint textureChecker = createTexture();
    glBindTexture(GL_TEXTURE_2D, textureChecker);

    glActiveTexture(GL_TEXTURE1);
    GLuint textureClippy = loadTexture("../img/clippy.png");
    glBindTexture(GL_TEXTURE_2D, textureClippy);

    glActiveTexture(GL_TEXTURE2);
    GLuint textureQuestion = loadTexture("../img/question.png");
    glBindTexture(GL_TEXTURE_2D, textureQuestion);

    glActiveTexture(GL_TEXTURE3);
    GLuint textureSmile = loadTexture("../img/smile.PNG");
    glBindTexture(GL_TEXTURE_2D, textureSmile);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(model, window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // render the object
        shader.use();
        Uniform::set(shader.id(), "model", model);
        Uniform::set(shader.id(), "projection", camera.projection);
        Uniform::set(shader.id(), "camera", camera.look_at());
        Uniform::set(shader.id(), "eye", camera.eye);

        //selects textures and shapes based on # of spacebar presses
        switch(spaceCount) {
            case 0:
                Uniform::set(shader.id(), "ourTexture", 0);
                glBindVertexArray(vao2);
                glDrawArrays(GL_TRIANGLES, 0, shape2.size());
                break;
            case 1:
                Uniform::set(shader.id(), "ourTexture", 1);
                glBindVertexArray(vao2);
                glDrawArrays(GL_TRIANGLES, 0, shape2.size());
                break;
            case 2:
                Uniform::set(shader.id(), "ourTexture", 2);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, shape.size());
                break;
            case 3:
                Uniform::set(shader.id(), "ourTexture", 3);
                glBindVertexArray(vao);
                glDrawArrays(GL_TRIANGLES, 0, shape.size());
                break;
            default:
                spaceCount = 0;
                break;
        }

        // render the cube
        //glBindVertexArray(vao);
        //glDrawArrays(GL_TRIANGLES, 0, shape.size());

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
