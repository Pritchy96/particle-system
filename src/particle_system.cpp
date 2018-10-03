// #include "stdafx.h"
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/particle_system.hpp"

using namespace glm;
using namespace std;

ParticleSystem::ParticleSystem(GLuint Shader, glm::vec3 origin, int numberOfParticles)
: Renderable(Shader) {
	particleCount = numberOfParticles;

    modelMatrix = glm::translate(mat4(1.0f), origin);

    for (int i = 0; i < numberOfParticles; i++) {
        vertexes.push_back(vec3(0.0f)); //Init all new particles at emitter origin.
        colours.push_back(vec3(0.4f, 1.0f, 0.3f));
    }
}

GLuint ParticleSystem::getTransBuffer() {
	if (isNewSystem) {
		glBindVertexArray(vao);

		vector<float> verts, cols;
		for (vector<glm::vec3>::const_iterator point = vertexes.begin(); point!=vertexes.end(); ++point) {
			verts.push_back(point->x);
			verts.push_back(point->y);
			verts.push_back(point->z); 
		}
		
		glEnableVertexAttribArray(2);
		glGenBuffers(1, &tb_current);
		glBindBuffer(GL_ARRAY_BUFFER, tb_current);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexes.size() * 4 * 2, verts.data(), GL_STATIC_READ);
		
		glEnableVertexAttribArray(3);
		glGenBuffers(1, &tb_previous);
		glBindBuffer(GL_ARRAY_BUFFER, tb_previous);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexes.size() * 4 * 2, verts.data(), GL_STATIC_READ);
	}
	
	std::swap(tb_current, tb_previous);
	return tb_current;
}

GLuint ParticleSystem::getPrevTBuf() {
	return tb_previous;
}