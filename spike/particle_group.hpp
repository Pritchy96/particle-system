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

    class Particle_Group : public Renderable {
        public:
            Particle_Group(GLuint Shader, vector<glm::vec3> vert_data, vector<glm::vec3> colour_data);
            Particle_Group(GLuint Shader, glm::vec3 Colour, glm::vec3 Position, glm::vec3 Acceleration, glm::vec3 Velocity, int numberOfParticles, float Lifespan, float particleSize);
            Particle_Group(GLuint Shader, glm::vec3 Colour, glm::vec3 Position, int numberOfParticles);         
            
            GLuint getVAO();   
            void generateVAO();
            
            glm::vec3 colour, position, acceleration, velocity;
	        float lifespan, size;
    };
#endif