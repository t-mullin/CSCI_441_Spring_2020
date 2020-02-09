// Created by Tristan on 2/8/2020.
// CSCI 441 Spring 2020
// David Millman
// 2/11/2020

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <csci441/shader.h>

#include "cmath"
#include "matrix3.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

int keypressCounter = 0;
//call back on keypress event
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //checks if the space key was released
    if(key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {
        keypressCounter++;
    }
}

int main(void) {
    /* Initialize the library */
    GLFWwindow* window;
    if (!glfwInit()) {
        return -1;
    }

#ifdef __APPLE__
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Lab 3", NULL, NULL);
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
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* init the triangle drawing */
    // define the vertex coordinates of the triangle
    float triangle[] = {
         0.5f,  0.5f, 1.0, 0.0, 0.0,
         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,

         0.5f, -0.5f, 1.0, 1.0, 0.0,
        -0.5f, -0.5f, 0.0, 0.0, 1.0,
        -0.5f,  0.5f, 0.0, 1.0, 0.0,
    };

    // create and bind the vertex buffer object and copy the data to the buffer
    GLuint VBO[1];
    glGenBuffers(1, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

    // create and bind the vertex array object and describe data layout
    GLuint VAO[1];
    glGenVertexArrays(1, VAO);
    glBindVertexArray(VAO[0]);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(0*sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 5*sizeof(float), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the shaders
    Shader shader("../vert.glsl", "../frag.glsl");

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // use the shader
        shader.use();

        /** Part 2 animate and scene by updating the transformation matrix */
        glfwSetKeyCallback(window, keyCallback);    //waits for key press
        float time = glfwGetTime(); //gets the current time as a float

        Matrix3 matrix;
        Matrix3 matrixR;
        Matrix3 matrixS;
        Matrix3 matrixT;
        //arrays with the rotation layouts
        float rotateZ[9] ={cosf(time), -sinf(time), 0.0f, sinf(time), cosf(time), 0.0f, 0.0f, 0.0f, 1.0f};  //change the first 2 values in the row to change the rotation axis;
        float rotateY[9] ={cosf(time), -sinf(time), 0.0f, 0.0f, sinf(time), 0.0f, 0.0f, 0.0f, 1.0f};  //change the first 2 values in the row to change the rotation axis;
        float rota[9] ={cosf(time), -sinf(time), 0.0f, sinf(time), cosf(time), 0.0f, -0.5f, -0.6f, 1.0f};  //change the first 2 values in the row to change the rotation axis;

        //translation needed to be transposed to work in opengl
        //translation layouts
        float translation[9] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, cosf(time), sinf(time), 1.0f};
        float translation2[9] = {1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -0.6f, 0.5f, 1.0f};

        int vertexLocation = glGetUniformLocation(shader.id(), "transform");
        //changes the scene depending on the keypress counter
        switch (keypressCounter) {
            case 1: //rotate around center
                matrix = Matrix3(rotateZ);
                break;
            case 2: //rotate off center
                matrix = Matrix3(translation2)*(Matrix3(rota)*(0.5*Matrix3()));
                break;
            case 3: //scale
                matrix = (sinf(time)*Matrix3());
                break;
            case 4: //madness
                matrixR = Matrix3(rotateY);
                matrixS = cosf(time)*Matrix3();
                matrixT = Matrix3(translation);
                matrix = matrixT*(matrixR*matrixS);
                break;
            case 5:
                keypressCounter = 0;
            default:
                break;
        }
        glUniformMatrix3fv(vertexLocation, 1, GL_FALSE, matrix.a);
        // draw our triangles
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES, 0, sizeof(triangle));

        /* Swap front and back * buffers */
        glfwSwapBuffers(window);

        /* Poll for and * process * events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
