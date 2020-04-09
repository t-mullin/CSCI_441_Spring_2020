

#include <iostream>
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

void render(bitmap_image& image, const std::vector<Sphere>& world) {
    // TODO: implement ray tracer
    const float l = -1.0f;
    const float r = 1.0f;
    const float b = -1.0f;
    const float t = -1.0f;

    float u, v;

    rgb_t bg_color = make_colour(181, 255, 196);

    for(int i = 0; i < IMG_WIDTH; i++) {
        for(int j = 0; j < IMG_HEIGHT; j++) {
            u = l + ((r - l)*(i + 0.5))/(float)IMG_WIDTH;
            v = b + ((t - b)*(j + 0.5))/(float)IMG_HEIGHT;

            image.set_pixel(i, j, bg_color);
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


