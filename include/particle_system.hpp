#ifndef PARTICLE_HPP
#define PARTICLE_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    #include "../include/renderable.hpp"
    #include "../include/shader.hpp"

    using namespace glm;
    using namespace std;

    class ParticleSystem : public Renderable{
        public:
            ParticleSystem();
            ParticleSystem(GLuint Shader, GLuint TransformShader);
            ParticleSystem(GLuint Shader, GLuint TransformShader, glm::vec3 origin, int numberOfParticles);
            
            virtual ~ParticleSystem();
            
            void Draw(float deltaT, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) override;            
            GLuint getVAO() override;
            GLuint getPrevVAO();

            int particleCount;
            float age;
            bool isNewSystem = true;
            GLuint transformShader;
		        GLuint vel_vbo, pos2_vbo, col2_vbo, vel2_vbo, age_vbo, age2_vbo; 
            GLuint vao2;
    };

#endif