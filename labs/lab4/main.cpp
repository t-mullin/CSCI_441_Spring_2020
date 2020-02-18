#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <csci441/shader.h>

#include "matrix3.h"
#include "vector3.h"
#include "matrix4.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//globals
int camYPos = 0;
int xPos = 0;
int yPos = 0;
int zPos = 0;
int scaler = 1;
int xStep = 0;
int yStep = 0;
int zStep = 0;
bool useOrthogonal = true;
int swapPrimitives = 0;


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window, Shader &shader) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    //Moves camera along the Y axis
    if(key == GLFW_KEY_W && action == GLFW_PRESS) {
        if(camYPos > -20) {
            camYPos--;
        }
    }
    if(key == GLFW_KEY_S && action == GLFW_PRESS) {
        if(camYPos < 20) {
            camYPos++;
        }
    }

    //translate x
    if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        if(xPos > -20) {
            xPos--;
        }
    }
    if(key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        if(xPos < 20) {
            xPos++;
        }
    }
    //translate y
    if(key == GLFW_KEY_UP && action == GLFW_PRESS) {
        if(yPos < 20) {
            yPos++;
        }
    }
    if(key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
        if(yPos > -20) {
            yPos--;
        }
    }
    //translate z
    if(key == GLFW_KEY_COMMA && action == GLFW_PRESS) {
        if(zPos < 20) {
            zPos++;
        }
    }
    if(key == GLFW_KEY_PERIOD && action == GLFW_PRESS) {
        if(zPos > -20) {
            zPos--;
        }
    }

    //rotate x
    if(key == GLFW_KEY_U && action == GLFW_PRESS) {
        xStep < 39 ? (xStep++) : (xStep = 0);
    }
    if(key == GLFW_KEY_I && action == GLFW_PRESS) {
        xStep > -39 ? (xStep--) : (xStep = 0);
    }
    //rotate y
    if(key == GLFW_KEY_O && action == GLFW_PRESS) {
        yStep < 39 ? (yStep++) : (yStep = 0);
    }
    if(key == GLFW_KEY_P && action == GLFW_PRESS) {
        yStep > -39 ? (yStep--) : (yStep = 0);
    }
    //rotate z
    if(key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS) {
        zStep < 39 ? (zStep++) : (zStep = 0);
    }
    if(key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS) {
        zStep > -39 ? (zStep--) : (zStep = 0);
    }
    //scale
    if(key == GLFW_KEY_MINUS && action == GLFW_PRESS) {
        if(scaler > -20) {
            scaler--;
        }
    }
    if(key == GLFW_KEY_EQUAL && action == GLFW_PRESS) {
        if(scaler < 20) {
            scaler++;
        }
    }

    //switch perspectives
    if(key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS) {
        useOrthogonal = !useOrthogonal;
    }

    //switch primitives
    if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        swapPrimitives++;
    }
    //resets

    if(key == GLFW_KEY_R && action == GLFW_PRESS) {
        camYPos = 0;

        xPos = 0;
        yPos = 0;
        zPos = 0;

        xStep = 0;
        yStep = 0;
        zStep = 0;

        scaler = 1;
    }
}

void errorCallback(int error, const char* description) {
    fprintf(stderr, "GLFW Error: %s\n", description);
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(errorCallback);

    /* Initialize the library */
    if (!glfwInit()) { return -1; }

#ifdef __APPLE__
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Lab 4", NULL, NULL);
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

    /* init the model */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
    };

    //cylinder
    const float PI = 3.14159265359;
    float cylinder[] = {
            //first half lower
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(0), 0.0f, 0.5f*cosf(0), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/6), 0.0f, 0.5f*cosf(PI/6), 1.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/6), 0.0f, 0.5f*cosf(PI/6), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/3), 0.0f, 0.5f*cosf(PI/3), 1.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/3), 0.0f, 0.5f*cosf(PI/3), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 0.0f, 0.5f*cosf(PI/2), 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 0.0f, 0.5f*cosf(PI/2), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI/3), 0.0f, 0.5f*cosf(2*PI/3), 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(2*PI/3), 0.0f, 0.5f*cosf(2*PI/3), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/6), 0.0f, 0.5f*cosf(5*PI/6), 1.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/6), 0.0f, 0.5f*cosf(5*PI/6), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI), 0.0f, 0.5f*cosf(PI), 1.0f, 1.0f, 1.0f,

            //second half lower
            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI), 0.0f, 0.5f*cosf(PI), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 0.0f, 0.5f*cosf(7*PI/6), 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 0.0f, 0.5f*cosf(7*PI/6), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(4*PI/3), 0.0f, 0.5f*cosf(4*PI/3), 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(4*PI/3), 0.0f, 0.5f*cosf(4*PI/3), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(3*PI/2), 0.0f, 0.5f*cosf(3*PI/2), 1.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(3*PI/2), 0.0f, 0.5f*cosf(3*PI/2), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/3), 0.0f, 0.5f*cosf(5*PI/3), 1.0f, 1.0f, 1.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/3), 0.0f, 0.5f*cosf(5*PI/3), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 0.0f, 0.5f*cosf(11*PI/6), 1.0f, 0.0f, 0.0f,

            0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 0.0f, 0.5f*cosf(11*PI/6), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI), 0.0f, 0.5f*cosf(2*PI), 1.0f, 0.0f, 0.0f,

            //first half upper
            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(0), 1.0f, 0.5f*cosf(0), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/6), 1.0f, 0.5f*cosf(PI/6), 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/6), 1.0f, 0.5f*cosf(PI/6), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI/3), 1.0f, 0.5f*cosf(PI/3), 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/3), 1.0f, 0.5f*cosf(PI/3), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 1.0f, 0.5f*cosf(PI/2), 1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 1.0f, 0.5f*cosf(PI/2), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI/3), 1.0f, 0.5f*cosf(2*PI/3), 1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(2*PI/3), 1.0f, 0.5f*cosf(2*PI/3), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/6), 1.0f, 0.5f*cosf(5*PI/6), 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/6), 1.0f, 0.5f*cosf(5*PI/6), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(PI), 1.0f, 0.5f*cosf(PI), 1.0f, 1.0f, 1.0f,

            //second half upper
            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(PI), 1.0f, 0.5f*cosf(PI), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 1.0f, 0.5f*cosf(7*PI/6), 1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 1.0f, 0.5f*cosf(7*PI/6), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(4*PI/3), 1.0f, 0.5f*cosf(4*PI/3), 1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(4*PI/3), 1.0f, 0.5f*cosf(4*PI/3), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(3*PI/2), 1.0f, 0.5f*cosf(3*PI/2), 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            0.5f*sinf(3*PI/2), 1.0f, 0.5f*cosf(3*PI/2), 1.0f, 1.0f, 1.0f,
            0.5f*sinf(5*PI/3), 1.0f, 0.5f*cosf(5*PI/3), 1.0f, 1.0f, 1.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/3), 1.0f, 0.5f*cosf(5*PI/3), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 1.0f, 0.5f*cosf(11*PI/6), 1.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 1.0f, 0.5f*cosf(11*PI/6), 1.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI), 1.0f, 0.5f*cosf(2*PI), 1.0f, 0.0f, 0.0f,

            //walls pt 1
            0.5f*sinf(0), 1.0f, 0.5f*cosf(0), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(0), 0.0f, 0.5f*cosf(0), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/6), 0.0f, 0.5f*cosf(PI/6), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(PI/6), 1.0f, 0.5f*cosf(PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/6), 0.0f, 0.5f*cosf(PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/3), 0.0f, 0.5f*cosf(PI/3), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(PI/3), 1.0f, 0.5f*cosf(PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/3), 0.0f, 0.5f*cosf(PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/2), 0.0f, 0.5f*cosf(PI/2), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(PI/2), 1.0f, 0.5f*cosf(PI/2), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI/2), 0.0f, 0.5f*cosf(PI/2), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(2*PI/3), 0.0f, 0.5f*cosf(2*PI/3), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(2*PI/3), 1.0f, 0.5f*cosf(2*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(2*PI/3), 0.0f, 0.5f*cosf(2*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(5*PI/6), 0.0f, 0.5f*cosf(5*PI/6), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(5*PI/6), 1.0f, 0.5f*cosf(5*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(5*PI/6), 0.0f, 0.5f*cosf(5*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI), 0.0f, 0.5f*cosf(PI), 0.0f, 1.0f, 0.0f,

            //walls pt 2
            0.5f*sinf(PI), 1.0f, 0.5f*cosf(PI), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(PI), 0.0f, 0.5f*cosf(PI), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(7*PI/6), 0.0f, 0.5f*cosf(7*PI/6), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(7*PI/6), 1.0f, 0.5f*cosf(7*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(7*PI/6), 0.0f, 0.5f*cosf(7*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(4*PI/3), 0.0f, 0.5f*cosf(4*PI/3), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(4*PI/3), 1.0f, 0.5f*cosf(4*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(4*PI/3), 0.0f, 0.5f*cosf(4*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(3*PI/2), 0.0f, 0.5f*cosf(3*PI/2), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(3*PI/2), 1.0f, 0.5f*cosf(3*PI/2), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(3*PI/2), 0.0f, 0.5f*cosf(3*PI/2), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(5*PI/3), 0.0f, 0.5f*cosf(5*PI/3), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(5*PI/3), 1.0f, 0.5f*cosf(5*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(5*PI/3), 0.0f, 0.5f*cosf(5*PI/3), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(11*PI/6), 0.0f, 0.5f*cosf(11*PI/6), 0.0f, 1.0f, 0.0f,

            0.5f*sinf(11*PI/6), 1.0f, 0.5f*cosf(11*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(11*PI/6), 0.0f, 0.5f*cosf(11*PI/6), 0.0f, 1.0f, 0.0f,
            0.5f*sinf(2*PI), 0.0f, 0.5f*cosf(2*PI), 0.0f, 1.0f, 0.0f,

            //walls pt 3
            0.5f*sinf(PI/6), 0.0f, 0.5f*cosf(PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(0), 1.0f, 0.5f*cosf(0), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/6), 1.0f, 0.5f*cosf(PI/6), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(PI/3), 0.0f, 0.5f*cosf(PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/6), 1.0f, 0.5f*cosf(PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/3), 1.0f, 0.5f*cosf(PI/3), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(PI/2), 0.0f, 0.5f*cosf(PI/2), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/3), 1.0f, 0.5f*cosf(PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 1.0f, 0.5f*cosf(PI/2), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(2*PI/3), 0.0f, 0.5f*cosf(2*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI/2), 1.0f, 0.5f*cosf(PI/2), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI/3), 1.0f, 0.5f*cosf(2*PI/3), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(5*PI/6), 0.0f, 0.5f*cosf(5*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI/3), 1.0f, 0.5f*cosf(2*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/6), 1.0f, 0.5f*cosf(5*PI/6), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(PI), 0.0f, 0.5f*cosf(PI), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/6), 1.0f, 0.5f*cosf(5*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI), 1.0f, 0.5f*cosf(PI), 0.0f, 0.0f, 0.0f,

            //walls pt 4
            0.5f*sinf(7*PI/6), 0.0f, 0.5f*cosf(7*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(PI), 1.0f, 0.5f*cosf(PI), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 1.0f, 0.5f*cosf(7*PI/6), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(4*PI/3), 0.0f, 0.5f*cosf(4*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(7*PI/6), 1.0f, 0.5f*cosf(7*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(4*PI/3), 1.0f, 0.5f*cosf(4*PI/3), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(3*PI/2), 0.0f, 0.5f*cosf(3*PI/2), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(4*PI/3), 1.0f, 0.5f*cosf(4*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(3*PI/2), 1.0f, 0.5f*cosf(3*PI/2), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(5*PI/3), 0.0f, 0.5f*cosf(5*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(3*PI/2), 1.0f, 0.5f*cosf(3*PI/2), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/3), 1.0f, 0.5f*cosf(5*PI/3), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(11*PI/6), 0.0f, 0.5f*cosf(11*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(5*PI/3), 1.0f, 0.5f*cosf(5*PI/3), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 1.0f, 0.5f*cosf(11*PI/6), 0.0f, 0.0f, 0.0f,

            0.5f*sinf(2*PI), 0.0f, 0.5f*cosf(2*PI), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(11*PI/6), 1.0f, 0.5f*cosf(11*PI/6), 0.0f, 0.0f, 0.0f,
            0.5f*sinf(2*PI), 1.0f, 0.5f*cosf(2*PI), 0.0f, 0.0f, 0.0f

    };

    // copy vertex data
    GLuint VBO1, VBO2;
    glGenBuffers(1, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // describe vertex layout
    GLuint VAO1;
    glGenVertexArrays(1, &VAO1);
    glBindVertexArray(VAO1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    //VBO and VAO for the cylinder
    glGenBuffers(1, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cylinder), cylinder, GL_STATIC_DRAW);

    GLuint VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    // create the shaders
    Shader shader("../vert.glsl", "../frag.glsl");

    // setup the textures
    shader.use();

    // and use z-buffering
    glEnable(GL_DEPTH_TEST);

    float aspectRatio = (float) SCREEN_WIDTH/ (float) SCREEN_HEIGHT;
    Vector3 gaze;
    Vector3 up;
    Vector3 eye;
    Matrix4 camera;
    Matrix4 orthographic;
    Matrix4 perspective;
    Matrix4 view;
    Matrix4 translate;
    Matrix4 rotate;
    Matrix4 scale;
    Matrix4 model;
    Matrix4 identity;

    //initializes the shader with the identity for the model and view matrix
    int vertexLocation = glGetUniformLocation(shader.id(), "model");
    glUniformMatrix4fv(vertexLocation, 1, GL_TRUE, identity.a);
    vertexLocation = glGetUniformLocation(shader.id(), "view");
    glUniformMatrix4fv(vertexLocation, 1, GL_TRUE, identity.a);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        // process input
        processInput(window, shader);
        glfwSetKeyCallback(window, keyCallback);    //waits for key press

        //
        if(useOrthogonal) {
            gaze = Vector3(0,0,0);
            up = Vector3(0,1,0);
            eye = Vector3(0, camYPos, 1);
            orthographic = buildOrthographic4(-aspectRatio,aspectRatio,-1,1, eye.a[2], -100);
            camera = buildCamera4(gaze, up, eye);
            view = orthographic*camera;
        } else {
            gaze = Vector3(0,0,0);
            up = Vector3(0,1,0);
            eye = Vector3(0, camYPos, 20);
            camera = buildCamera4(gaze, up, eye);
            perspective = buildPerspective4(SCREEN_WIDTH,SCREEN_HEIGHT, 60.0, 1, -100.0);
            view = perspective*camera;
        }

        //builds the component of the model matrix
        translate = buildTranslation4(xPos, yPos, zPos);
        rotate = buildRotationX4(xStep)*buildRotationY4(yStep)*buildRotationZ4(zStep);
        scale = buildScale4(scaler);
        //builds the model matrix
        model = translate*rotate*scale;

        /* Render here */
        glClearColor(0.2f, 0.6f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // activate shader
        shader.use();

        vertexLocation = glGetUniformLocation(shader.id(), "model");
        glUniformMatrix4fv(vertexLocation, 1, GL_TRUE, model.a);
        vertexLocation = glGetUniformLocation(shader.id(), "view");
        glUniformMatrix4fv(vertexLocation, 1, GL_TRUE, view.a);

        switch(swapPrimitives) {
            case 0:
                // render the cube
                glBindVertexArray(VAO1);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
                break;
            case 1:
                // render the cylinder
                glBindVertexArray(VAO2);
                glDrawArrays(GL_TRIANGLES, 0, sizeof(cylinder));
                break;
            default:
                swapPrimitives = 0;
                break;
        }

        /* Swap front and back and poll for io events */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
