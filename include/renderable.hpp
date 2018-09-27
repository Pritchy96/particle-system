#ifndef RENDERABLE_HPP
#define RENDERABLE_HPP

    // #include "stdafx.h"
    #include <vector>
    #include <GL/glew.h>
    #include <GLFW/glfw3.h>
    #include <glm/glm.hpp>
    #include <glm/gtc/matrix_transform.hpp>

    using namespace glm;
    using namespace std;

    class Renderable {
        public:
            Renderable();
            Renderable(GLuint Shader);
            Renderable(GLuint Shader, vector<glm::vec3> vert_data);
            Renderable(GLuint Shader, vector<glm::vec3> vert_data, vector<glm::vec3> colour_data);

            GLuint getVAO();   

            vector<vec3> vertexes, colours;
	        GLuint pos_vbo, col_vbo, vao, shader;
            glm::mat4 modelMatrix = glm::mat4(1.0f);
            bool validVAO = false;
    };

#endif