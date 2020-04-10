

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

int findIntersection(Ray ray, float t_0, float t_1, const std::vector<Sphere>& world) {
    int hitObj = 0;
    double a = glm::dot(ray.direction, ray.direction);
    double b = glm::dot(2.0f*ray.direction, ray.origin);
    double c;
    double d_2;
    double solu1;
    double solu2;
    //need to shift the points based on the center location of the sphere
    for(int i = 0; i < world.size(); i++) {
        c = glm::dot(ray.origin, ray.origin) - powf(world[i].radius,2);
        d_2 = pow(b, 2) - (4*a*c);
        if(d_2 < 0) {
            hitObj = 0;
        } else if (d_2 == 0) {
            solu1 = -b/(2*a);
        } else if (d_2 > 0) {
            solu1 = (-b + sqrt(d_2))/(2*a);
            solu2 = (-b - sqrt(d_2))/(2*a);
        }
    }
    return hitObj;
}

void render(bitmap_image& image, const std::vector<Sphere>& world) {
    // TODO: implement ray tracer
    Ray ray;
    float ui;
    float vj;
    int hit;

    Viewport viewport = Viewport(glm::vec2(-1, -1), glm::vec2(1, 1));
    rgb_t bg_color = make_colour(181, 255, 196);

    for(int i = 0; i < IMG_WIDTH; i++) {
        for(int j = 0; j < IMG_HEIGHT; j++) {
            ui = viewport.min.x + (viewport.max.x - viewport.min.x)*(i + 0.5) / IMG_WIDTH;
            vj = viewport.min.y + (viewport.max.y - viewport.min.y)*(j + 0.5) / IMG_HEIGHT;

            ray.origin = glm::vec3(ui, vj, 0);
            ray.direction = glm::vec3(0,0,-1);

            hit = findIntersection(ray, 0, INFINITY, world);

            if(hit > 0) {
                image.set_pixel(i, j, make_colour(world[hit-1].color.r, world[hit-1].color.g,world[hit-1].color.b));
            } else {
                image.set_pixel(i, j, bg_color);
            }
        }
    }

}

int main(int argc, char** argv) {

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(IMG_WIDTH, IMG_HEIGHT);

    // build world
    std::vector<Sphere> world = {
        Sphere(glm::vec3(0, 0, 1), 1, glm::vec3(1,1,0)),
        Sphere(glm::vec3(1, 1, 4), 2, glm::vec3(0,1,1)),
        Sphere(glm::vec3(2, 2, 6), 3, glm::vec3(1,0,1)),
    };

    // render the world
    render(image, world);

    image.save_image("../img/ray-traced.bmp");
    std::cout << "Success" << std::endl;
}


