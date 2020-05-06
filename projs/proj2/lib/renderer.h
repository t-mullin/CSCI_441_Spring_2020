// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 5/6/2020

//Code provided by David Millman
//Modifed the phong, shade, and render methods to add reflections, shadows, and anti-aliasing

#ifndef _CSCI441_RENDERER_H_
#define _CSCI441_RENDERER_H_

#include "camera.h"
#include "hit.h"
#include "light.h"
#include "ray.h"
#include "intersector.h"
#include "shader.h"
#include <ctime>
#include <random>


class Renderer {

    Intersector* _intersector;

    int clamp255(float v) {
        return std::max(0.0f, std::min(255.0f, 255.0f*v));
    }


    rgb_t to_color(const glm::vec3 c) {
        return make_colour(clamp255(c.x), clamp255(c.y), clamp255(c.z));
    }


    glm::vec3 phong(const Hit& hit, const Light* light, const glm::vec3& eye, const World& world, int reflectionDepth) {

        //Coefficients
        float ambient_coefficient = 0.2;
        float diffuse_coefficient = 1;
        float specular_coefficient = 0.8;
        float shinyness = 2.0;

        glm::vec3 pos = hit.position();
        glm::vec3 normal = hit.normal();    //n
        glm::vec3 light_dir = glm::normalize(light->direction(pos)); //l
        glm::vec3 view_dir = glm::normalize(eye - pos); //v
        glm::vec3 h = glm::normalize(-light_dir + (-view_dir)); //h

        float ambient = ambient_coefficient;
        float diffuse = diffuse_coefficient * glm::max(glm::dot(normal, light_dir), 0.0f);
        float specular = specular_coefficient * pow(glm::max(glm::dot(normal, h), 0.0f), shinyness);

        float e = 0.000003; //small offset

        /* Start of shadow tracing */
        Ray shadowRay;
        shadowRay.origin = e+pos; //hit point moved over a bit
        shadowRay.direction = light_dir; //points toward the light source

        Hit shadowHit = _intersector->find_first_intersection(world, shadowRay);

        if(shadowHit.is_intersection()) {
            diffuse = 0.0f;
            specular = 0.0f;
        }
        /* end of shadow tracing */

        /* start of reflection tracing */
        Ray reflectionRay;
        //ray origin is offset by e to prevent colliding with the object that the ray is bouncing off of
        reflectionRay.origin = e+pos; //hit point moved over a bit
        reflectionRay.direction = -view_dir - (2*(glm::dot(-view_dir,normal))*normal); //the direction of the refection ray

        Hit reflectionHit = _intersector->find_first_intersection(world, reflectionRay);

        //checks if to see if there is something to reflect off of and if we have reached the mex recursion depth
        if(reflectionHit.is_intersection() && reflectionDepth != 0) {
            //recursion to get the reflection colors
            return hit.color() = 0.5f*phong(reflectionHit, light, eye, world, reflectionDepth-1);
        }
        /* end of reflection tracing */

        glm::vec3 light_color = (ambient+diffuse+specular) * light->attenuation(pos) * light->color();

        return light_color*hit.color();
    }


    glm::vec3 shade(const Camera& camera, const Lights& lights, const Hit& hit, const World& world) {
        int recursionDepth = 1;
        glm::vec3 color = camera.background;
        if (hit.is_intersection()) {
            color = glm::vec3(0,0,0);
            for (auto light : lights) {
                color += phong(hit, light, camera.pos, world, recursionDepth);
            }
        }
        return color;
    }

    glm::vec3 render_pixel(const Camera& camera, const Lights& lights, const World& world, const Ray& ray) {

        Hit hit = _intersector->find_first_intersection(world, ray);
        return shade(camera, lights, hit, world);
    }

public:

    Renderer(Intersector* intersector) : _intersector(intersector) { }

    void render(bitmap_image &image, const Camera& camera, const Lights& lights, const World& world) {
        static std::default_random_engine generator; //generator for the random number generator
        //using a uniform real distribution as it can lay in the range of [0.0, 1.0)
        std::uniform_real_distribution<double> distribution(0.0, 1.0);
        int numSamples = 10; //sets the number of n samples per pixel

        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                /* start of anti-aliasing */
                glm::vec3 c = glm::vec3(0,0,0);
                if(numSamples != 0) {
                    //two for-loops to generate n^2 samples per pixel
                    for (int p = 0; p < numSamples; p++) {
                        for (int q = 0; q < numSamples; q++) {
                            //j1 & j2 are the jitter factors to sample in the correct range
                            double j1 = (p + distribution(generator)) / (double) numSamples;
                            double j2 = (q + distribution(generator)) / (double) numSamples;
                            //generates a ray that points to a subsection of the pixel to be sampled
                            Ray ray = camera.make_ray(image.width(), image.height(), x, y, j1, j2);
                            //adds the previous color with the new color
                            c = c + render_pixel(camera, lights, world, ray);
                        }
                    }
                    //gets the average color from all of the samples
                    c = c/powf(numSamples, 2);
                } else {
                    //if the number of samples is 0, then take one sample from the middle of the pixel
                    Ray ray = camera.make_ray(image.width(), image.height(), x, y, 0.5, 0.5);
                    c = render_pixel(camera, lights, world, ray);
                }
                /* end of anti-aliasing */
                image.set_pixel(x, image.height()-y-1, to_color(c));
            }
        }
    }
};

#endif
