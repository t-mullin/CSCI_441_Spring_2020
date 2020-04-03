// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>

#include "lib/model.h"
#include "lib/light.h"
#include "lib/matrix4.h"
#include "lib/vector4.h"
#include "lib/camera.h"
#include "lib/uniform.h"
#include "lib/renderer.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;
bool birdeye = false;
float Rot = 0.0f;
float RotY = 0.0f;

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
    const float TRANS = .01;

    if(birdeye) {
        if (isPressed(window, GLFW_KEY_W)) {
            trans = rotate_y(ROT) * translate(TRANS,0,0) * rotate_y(-ROT);
        } else if(isPressed(window, GLFW_KEY_S)) {
            trans = rotate_y(ROT) * translate(-TRANS,0,0) * rotate_y(-ROT);
        } else if(isPressed(window, GLFW_KEY_A)) {
            trans=rotate_y(-ROT);
            if(abs(Rot) == 360.0F) {
                Rot = 0.0f;
            } else {
                Rot--;
            }
        } else if(isPressed(window, GLFW_KEY_D)) {
            trans=rotate_y(ROT);
            if(abs(Rot) == 360.0F) {
                Rot = 0.0f;
            } else {
                Rot++;
            }
        }
    } else {
        if (isPressed(window, GLFW_KEY_W)) {
            trans=translate(TRANS,0, 0);
        } else if(isPressed(window, GLFW_KEY_S)) {
            trans=translate(-TRANS,0, 0);
        } else if(isPressed(window, GLFW_KEY_A)) {
            trans=rotate_y(ROT);
            if(abs(Rot) == 360.0F) {
                Rot = 0.0f;
            } else {
                Rot++;
            }
        } else if(isPressed(window, GLFW_KEY_D)) {
            trans=rotate_y(-ROT);
            if(abs(Rot) == 360.0F) {
                Rot = 0.0f;
            } else {
                Rot--;
            }
        } else if(isPressed(window, GLFW_KEY_UP)) {
            if(RotY == 0) {
                RotY = 0;
            } else {
                RotY--;
            }
        } else if(isPressed(window, GLFW_KEY_DOWN)) {
            if(RotY == 90) {
                RotY = 90;
            } else {
                RotY++;
            }
        }
    }
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
        if(!birdeye) {
            birdeye = true;
        } else {
            birdeye = false;
        }
    }
    model = processModel(model, window);
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
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
    Camera camera = Camera(Vector4(0, 0, 3), Vector4(0, 0, 0), Vector4(0, 1, 0));
    camera.perspective = perspective(SCREEN_WIDTH, SCREEN_HEIGHT, 90, .01, 10);;
    camera.orthogonal = orthographic(-5.0f, 5.0f, -5.0f, 5.0f, 2.0f, -100.0f);;

    Model maze = Model("../models/testMaze.obj", "../textures/Hedge512.png", 0,Shader("../shaders/mazeVert.glsl", "../shaders/mazeFrag.glsl"),true, true);
    Model ground = Model("../models/ground.obj",  "../textures/Path512.png", 1,Shader("../shaders/groundVert.glsl", "../shaders/groundFrag.glsl"), true, true);
    Model dino = Model("../models/dino.obj",  "../textures/Path512.png", 2,Shader("../shaders/vert.glsl", "../shaders/frag.glsl"), false, false);
    Model duck = Model("../models/duck.obj",  "../textures/Path512.png", 3,Shader("../shaders/vert.glsl", "../shaders/frag.glsl"), false, false);

    Vector4 dinoPos;
    Vector4 camPos;

    //placing the duck
    Matrix4 trans;
    trans = translate(-3.2, 0.2, 4.05);
    duck.model = trans*duck.model;
    trans = rotate_y(180);
    duck.model = trans*duck.model;
    trans = scale(0.0002f);
    duck.model = trans*duck.model;

    //placing the player dino
    trans = translate(-2.95, 0.2, -3.2);
    dino.model = trans*dino.model;
    trans = scale(0.8f);
    dino.model = trans*dino.model;

    trans = translate(0, 0.005, 0);
    ground.model = trans*ground.model;

    //Light
    Light light = Light(Vector4(0, 10.0, 0), Vector4(1.0f, 1.0f, 1.0f), 1.0f);

    Renderer renderer;

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(dino.model, window);

        /* Render here */
        glClearColor(0.2f, 0.6f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render the object
        if(birdeye) {
            camera.eye = Vector4(0, -1, 0);
            camera.origin = Vector4(0, 0, 0);
            camera.up = Vector4(0, 0, 1);
            camera.lookAt = camera.look_at();
        } else {
            dinoPos = Vector4(dino.model.values[12], dino.model.values[13], dino.model.values[14]);
            camPos = -0.35*rotate_y(Rot, Vector4(1, 0, 1, 0)) + (0.3*rotate_z(RotY, Vector4(0, 1, 0, 0)));
            camera.eye = dinoPos+camPos;
            camera.origin = dinoPos;
            camera.up = Vector4(0, 1, 0);
            camera.lookAt = camera.look_at();
        }

        renderer.render(camera, dino, light, birdeye);
        renderer.render(camera, duck, light, birdeye);
        renderer.render(camera, maze, light, birdeye);
        renderer.render(camera, ground, light, birdeye);

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
