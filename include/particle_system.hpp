#ifndef PARTICLE_HPP
#define PARTICLE_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class ParticleSystem : public Renderable{
        public:
            ParticleSystem();
            ParticleSystem(GLuint Shader);
            ParticleSystem(GLuint Shader, glm::vec3 origin, int numberOfParticles);
            GLuint getTransBuffer();
            GLuint getPrevTBuf();

            int particleCount;
            bool isNewSystem = true;
            GLuint tb_current, tb_previous;
    };

#endif