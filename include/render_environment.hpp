#ifndef RENDERENVIRONMENT_HPP
#define RENDERENVIRONMENT_HPP

    // #include "stdafx.h"
    #include <vector>

    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>
    #include "../include/renderable.hpp"

    using namespace glm;
    using namespace std;

    class renderEnvironment {
        public:
            renderEnvironment();
            ~renderEnvironment();
            void addRenderable(Renderable renderable);
            void update();
            static void error_callback(int error, const char* description);
            static void window_size_callback(GLFWwindow* window, int width, int height);
            static void update_fps_counter(GLFWwindow* window);

            vector<Renderable> renderables;            
    };

#endif