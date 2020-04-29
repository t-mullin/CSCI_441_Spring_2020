#ifndef _CSCI441_RENDERER_H_
#define _CSCI441_RENDERER_H_

#include "camera.h"
#include "hit.h"
#include "light.h"
#include "ray.h"
#include "intersector.h"
#include "shader.h"

class Renderer {

    Intersector* _intersector;

    int clamp255(float v) {
        return std::max(0.0f, std::min(255.0f, 255.0f*v));
    }


    rgb_t to_color(const glm::vec3 c) {
        return make_colour(clamp255(c.x), clamp255(c.y), clamp255(c.z));
    }


    glm::vec3 phong(const Hit& hit, const Light* light, const glm::vec3& eye, const World& world) {

        float ambient_coefficient = 0.2;
        float diffuse_coefficient = 1.0;
        float specular_coefficient = 0.5;
        float shinyness = 2.0;

        glm::vec3 pos = hit.position();

        glm::vec3 normal = hit.normal();    //n
        glm::vec3 light_dir = glm::normalize(light->direction(pos)); //l
        glm::vec3 view_dir = glm::normalize(eye - pos); //v

        float ambient = ambient_coefficient;
        glm::vec3 h = glm::normalize(-light_dir + (-view_dir)); //h
        float diffuse = diffuse_coefficient * glm::max(glm::dot(normal, light_dir), 0.0f);
        float specular = specular_coefficient * pow(glm::max(glm::dot(normal, h), 0.0f), shinyness);

        float e = 0.000003;
        Ray shadowRay;
        shadowRay.origin = e+pos;
        shadowRay.direction = light_dir;

        Hit shadowHit = _intersector->find_first_intersection(world, shadowRay);

        if(shadowHit.is_intersection()) {
            diffuse = 0.0f;
            specular = 0.0f;
        }

        glm::vec3 light_color = (ambient+diffuse+specular) * light->attenuation(pos) * light->color();

        return light_color*hit.color();
    }


    glm::vec3 shade(const Camera& camera, const Lights& lights, const Hit& hit, const World& world) {
        glm::vec3 color = camera.background;
        if (hit.is_intersection()) {
            color = glm::vec3(0,0,0);
            for (auto light : lights) {
                color += phong(hit, light, camera.pos, world);
            }
        }
        return color;
    }

    glm::vec3 render_pixel(
        const Camera& camera,
        const Lights& lights,
        const World& world,
        const Ray& ray
    ) {

        Hit hit = _intersector->find_first_intersection(world, ray);
        return shade(camera, lights, hit, world);
    }

public:

    Renderer(Intersector* intersector) : _intersector(intersector) { }

    void render(
        bitmap_image &image,
        const Camera& camera,
        const Lights& lights,
        const World& world
    ) {
        for (int y = 0; y < image.height(); ++y) {
            for (int x = 0; x < image.width(); ++x) {
                Ray ray = camera.make_ray(image.width(), image.height(), x, y);
                glm::vec3 c = render_pixel(camera, lights, world, ray);
                image.set_pixel(x, image.height()-y-1, to_color(c));
            }
        }
    }
};

#endif
