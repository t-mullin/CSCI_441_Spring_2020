// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/7/2020
/*
 * Reused and modified the Renderer class provided by David Millman
 */


#ifndef PROJ1_RENDERER_H
#define PROJ1_RENDERER_H

#include "camera.h"

class Renderer {

public:

    void render(Camera camera, Model& m, Light light, bool useOrtho) {
        m.shader.use();
        Uniform::set(m.shader.id(), "model", m.model);
        if(useOrtho) {
            Uniform::set(m.shader.id(), "projection", camera.orthogonal);
        } else {
            Uniform::set(m.shader.id(), "projection", camera.perspective);
        }
        Uniform::set(m.shader.id(), "camera", camera.lookAt);
        Uniform::set(m.shader.id(), "eye", camera.eye);
        Uniform::set(m.shader.id(), "lightSource", light.lightPosition);
        Uniform::set(m.shader.id(), "lightColor", light.lightColor);
        Uniform::set(m.shader.id(), "lightIntensity", light.lightIntensity);
        Uniform::set(m.shader.id(), "ourTexture", m.textureIndex);

        // render the cube
        glBindVertexArray(m.VAO);
        glDrawElements(GL_TRIANGLES, m.indices.size(), GL_UNSIGNED_INT, 0);
    }

};

#endif //PROJ1_RENDERER_H
