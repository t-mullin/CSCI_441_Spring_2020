
#include <iostream>
#include <ctime>
#include <cmath>

#include <glm/glm.hpp>

#include <bitmap/bitmap_image.hpp>

#include "lib/camera.h"
#include "lib/hit.h"
#include "lib/intersector.h"
#include "lib/light.h"
#include "lib/ray.h"
#include "lib/renderer.h"
#include "lib/shape.h"
#include "lib/timer.h"
#include "lib/shader.h"


class BruteForceIntersector : public Intersector {
public:

    Hit find_first_intersection(const World& world, const Ray& ray) {
        Hit hit(ray);
        for (auto surface : world.shapes()) {
            double cur_t = surface->intersect(ray);

            if (cur_t < hit.t()) {
                hit.update(surface, cur_t);
            }
        }
        return hit;
    }
};


class MySlickIntersector : public Intersector {
public:
    Hit find_first_intersection(const World& world, const Ray& ray) {
        Hit hit(ray);
        // TODO: accelerate finding intersections with a spatial data structure.
        return hit;
    }
};


double rand_val() {
    static bool init = true;
    if (init) {
        srand(time(NULL));
        init = false;
    }
    return ((double) rand() / (RAND_MAX));
}

glm::vec3 rand_color() {
    return glm::vec3(rand_val(),rand_val(),rand_val());
}


std::vector<Triangle> random_box() {
    float  x = (rand_val() * 8) - 4;
    float  y = (rand_val() * 8) - 4;
    float  z = rand_val() * 5;
    float scale = rand_val() * 2;
    return Obj::make_box(glm::vec3(x, y, z), scale, rand_color());
}

bool boxIntersection(const std::vector<Triangle>& aab, const Ray& ray) {
    for(int i = 0; i < aab.size(); i++) {
        if(aab[i].intersect(ray, 0, INFINITY) != INFINITY) {
            return true;
        }
    }
    return false;
}

std::vector<Triangle> ground_plane() {
    return Obj::make_box(glm::vec3(0, -7, 0), 10, rand_color());
}

int main(int argc, char** argv) {

    // set the number of boxes
    int NUM_BOXES = 2;

    // create an image 640 pixels wide by 480 pixels tall
    bitmap_image image(640, 480);

    // setup the camera
    float dist_to_origin = 5;
    Camera camera(
            glm::vec3(0, 0, -dist_to_origin),   // eye
            glm::vec3(0, 0, 0),                 // target
            glm::vec3(0, 1, 0),                 // up
            glm::vec2(-5, -5),                  // viewport min
            glm::vec2(5, 5),                    // viewport max
            dist_to_origin,                     // distance from eye to view plane
            glm::vec3(.3, .4, .8)               // background color
    );

    // setup lights
    // see http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
    // for good attenuation value.
    // I found the values at 7 to be nice looking
    PointLight l1(glm::vec3(1, 1, 1), glm::vec3(5, 5, 0), 1.0, .7, 0.18);
    PointLight l3(glm::vec3(1, 1, 1), glm::vec3(-5, 5, 0), 1.0, .7, 0.18);
    DirectionalLight l2(glm::vec3(.5, .5, .5), glm::vec3(0, -4, 0));
    Lights lights = { &l1, &l2, &l3 };

    // setup world
    World world;

    // add the light
    //world.append(Sphere(l1.position(), .25, glm::vec3(1,1,1)));

    // and the spheres
    world.append(Sphere(glm::vec3(0, -1, 0), 1, rand_color()));
    world.append(Sphere(glm::vec3(2, 0, 0), 1, rand_color()));
    world.append(ground_plane());
    //world.append(Sphere(glm::vec3(2, 2, 4), 2, rand_color()));
    //world.append(Sphere(glm::vec3(3, 3, 6), 3, rand_color()));

    // and add some boxes and prep world for rendering
    //for (int i = 0 ; i < NUM_BOXES ; ++i) {
    //    world.append(random_box());
    //}
    world.lock();

    // create the intersector
    BruteForceIntersector intersector;

    // and setup the renderer
    Renderer renderer(&intersector);

    // render
    Timer timer;
    timer.start();
    renderer.render(image, camera, lights, world);
    timer.stop();

    image.save_image("../img/ray-traced_active.bmp");
    std::cout << "Rendered in " <<  timer.total() << " milliseconds" << std::endl;
}


