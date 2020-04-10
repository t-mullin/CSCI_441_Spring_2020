// Created by Tristan Mullin.
// CSCI 441 Spring 2020
// David Millman
// 4/14/2020

#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <bitmap/bitmap_image.hpp>
#include "lib/ray.h"

const int IMG_WIDTH = 640;
const int IMG_HEIGHT = 480;

struct Viewport {
    glm::vec2 min;
    glm::vec2 max;

    Viewport(const glm::vec2& min, const glm::vec2& max)
        : min(min), max(max) {}
};

struct Sphere {
    int id;
    glm::vec3 center;
    glm::vec3 color;
    float radius;

    Sphere(const glm::vec3& center=glm::vec3(0,0,0),
            float radius=0,
            const glm::vec3& color=glm::vec3(0,0,0))
        : center(center), radius(radius), color(color) {
            static int id_seed = 0;
            id = ++id_seed;
        }
};

//checks to see if the ray intersects any of the objects in the scene
//if thee is a hit, set the hitObj to the object id
//based off Paul Rademacher's notes on traytracing
int findIntersection(Ray ray, float t_0, float t_1, const std::vector<Sphere>& world) {
    int hitObj = 0;
    glm::vec3 EO;
    float v;
    float disk;
    float d;
    float t = t_1;

    for(int i = 0; i < world.size(); i++) {
        EO = world[i].center - ray.origin;
        v = glm::dot(EO, ray.direction);
        disk = powf(world[i].radius, 2) - (glm::dot(EO,EO) - powf(v, 2));
        if(disk >= 0) {
            d = sqrtf(disk);
            if(abs(v-d) < t) {
                t = abs(v - d);
                hitObj = world[i].id;
            }
        }
    }
    return hitObj;
}

void render(bitmap_image& image, const std::vector<Sphere>& world, bool useOrtho) {
    // TODO: implement ray tracer
    Ray ray;
    float ui;
    float vj;
    float d = 9;
    int hit;

    Viewport viewport = Viewport(glm::vec2(-5, -5), glm::vec2(5, 5));

    rgb_t bg_color = make_colour(143, 98, 255);
    rgb_t sphere_color;

    for(int i = 0; i < IMG_WIDTH; i++) {
        for(int j = 0; j < IMG_HEIGHT; j++) {
            ui = viewport.min.x + (viewport.max.x - viewport.min.x)*(i + 0.5) / IMG_WIDTH;
            vj = viewport.min.y + (viewport.max.y - viewport.min.y)*(j + 0.5) / IMG_HEIGHT;

            //checks if using orthographic or perspective projection
            if(useOrtho) {
                ray.origin = glm::vec3(ui, -vj, 0);
                ray.direction = glm::vec3(0, 0, -1);
            } else {
                ray.origin = glm::vec3(-2,-1, -d);
                ray.direction = glm::normalize(glm::vec3(ui, -vj, d));
            }

            hit = findIntersection(ray, 0, INFINITY, world);

            //if there was a hit, then set the pixel color to the object color else get the pixel color to the bg color
            if(hit > 0) {
                sphere_color = make_colour(255*world[hit-1].color.r, 255*world[hit-1].color.g,255*world[hit-1].color.b);
                image.set_pixel(i, j, sphere_color);
            } else {
                image.set_pixel(i, j, bg_color);
            }
        }
    }

}

int main(int argc, char** argv) {

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(IMG_WIDTH, IMG_HEIGHT);

    // build world full of spheres
    std::vector<Sphere> world = {
        Sphere(glm::vec3(0, 0, 0), 1, glm::vec3(13.0/255.0,255.0/255.0,202.0/255.0)),
        Sphere(glm::vec3(1, -1.5, 3), 2, glm::vec3(240.0/255.0,25.0/255.0,255.0/255.0)),
        Sphere(glm::vec3(3, 2, 6), 3, glm::vec3(0,255.0/255.0,55.0/255.0)),
        Sphere(glm::vec3(-2, 0, 12), 1, glm::vec3(255.0/255.0,85.0/255.0,25.0/255.0)),
        Sphere(glm::vec3(-4, -3, 5), 3, glm::vec3(117.0/255.0,255.0/255.0,13.0/255.0)),
        Sphere(glm::vec3(5, -5, 7), 2, glm::vec3(13.0/255.0,255.0/255.0,202.0/255.0)),
        Sphere(glm::vec3(-2.5, 2.5, 3), 1.5, glm::vec3(103.0/255.0,25.0/255.0,255.0/255.0)),
    };

    // render the world
    render(image, world, true);
    image.save_image("../img/orthographic.bmp");

    render(image, world, false);
    image.save_image("../img/perspective.bmp ");


    std::cout << "Success" << std::endl;
}


