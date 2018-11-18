#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    // #include "stdafx.h"
    #include <vector>

    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "../include/renderable.hpp"
    #include "../include/particle_system.hpp"

    using namespace glm;
    using namespace std;

    class renderEnvironment {
        public:
            renderEnvironment();
            ~renderEnvironment();

            void addRenderable(Renderable* renderable);
            void update(float deltaT);
            void setupTransformShader(GLuint transformShader);   
            
            static void errorCallback(int error, const char* description);
            static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
            static void windowSizeCallback(GLFWwindow* window, int width, int height);
            void setRenderWindowTitle(GLFWwindow* window, double deltaT);
            
            vector<Renderable*> renderables;
            GLuint basicShader;
            GLuint particleShader;
            GLuint transformShader;
            GLFWwindow* window;
    };

#endif