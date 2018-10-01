#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    using namespace glm;
    using namespace std;

    class Particle_System {
        public:
            Particle_System();
            Particle_System(GLuint Shader, int numberOfParticles);

            GLuint getTransBuffer(), getPrevTBuf();

            GLuint tb_current, tb_previous;
            vector<vec3> vertexes, colours;
	        GLuint pos_vbo, col_vbo, vao, shader;
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            int particleCount;
            bool isNewSystem = true;
    };

#endif