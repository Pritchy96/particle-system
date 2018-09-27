#ifndef PARTICLE_HPP
#define PARTICLE_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    // #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class Particle : public glm::vec3{
        public:
            Particle(glm::vec3 Colour, glm::vec3 Position, glm::vec3 Acceleration, glm::vec3 Velocity, float Lifespan, float particleSize );

            glm::vec3 colour, position, acceleration, velocity;
	        float lifespan, size;
    };

#endif