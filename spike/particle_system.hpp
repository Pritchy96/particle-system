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
            ParticleSystem(GLuint Shader, vector<glm::vec3> vert_data);
            ParticleSystem(GLuint Shader, vector<glm::vec3> vert_data, vector<glm::vec3> colour_data);

            vector<vec3> vertexes, colours;
	        GLuint pos_vbo, col_vbo, vao, shader;
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            bool validVAO = false;
    };

#endif