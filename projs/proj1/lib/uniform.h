// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020
/*
 * Reused and modified the Uniform class provided by David Millman
 */

#ifndef PROJ1_UNIFORM_H
#define PROJ1_UNIFORM_H

#include "matrix4.h"
#include "vector4.h"

class Uniform {

public:

    static void set(GLuint shader, const std::string &name, const Matrix4& m) {
        GLuint loc = location(shader, name);
        glUniformMatrix4fv(loc, 1, GL_FALSE, m.values);
    }

    static void set(GLuint shader, const std::string &name, const Vector4& v) {
        GLuint loc = location(shader, name);
        glUniform3f(loc, v.values[0], v.values[1], v.values[2]);
    }

    static void set(GLuint shader, const std::string &name, const int v) {
        GLuint loc = location(shader, name);
        glUniform1i(loc, v);
    }

    static void set(GLuint shader, const std::string &name, const float v) {
        GLuint loc = location(shader, name);
        glUniform1f(loc, v);
    }

private:

    static GLuint location(GLuint shader, const std::string &name) {
        return glGetUniformLocation(shader, name.c_str());
    }

};


#endif //PROJ1_UNIFORM_H
