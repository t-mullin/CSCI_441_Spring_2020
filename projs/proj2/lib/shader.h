//
// Created by Tristan on 4/28/2020.
//

#ifndef PROJ2_SHADER_H
#define PROJ2_SHADER_H

#include <glm/glm.hpp>

class Shader {
public:
    glm::vec3 shadow;


    glm::vec3 generate_shadows();
};




#endif //PROJ2_SHADER_H
