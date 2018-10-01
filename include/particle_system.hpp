#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class Particle_System : public Renderable {
        public:
            Particle_System();
            Particle_System(vec3 Position, vec3 Colour, GLuint Shader, int numberOfParticles);

            GLuint getTransBuffer(), getPrevTBuf();

            GLuint tb_current, tb_previous;
            int particleCount;
            bool isNewSystem = true;
    };

#endif