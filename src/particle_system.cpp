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

        cout << glm::to_string(colours[i]) << endl;
    }
}

GLuint ParticleSystem::getTransBuffer() {
	if (isNewSystem) {
		glGenBuffers(1, &tb_current);
		glBindBuffer(GL_ARRAY_BUFFER, tb_current);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * particleCount, nullptr, GL_STATIC_READ);
		
		glGenBuffers(1, &tb_previous);
		glBindBuffer(GL_ARRAY_BUFFER, tb_previous);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * particleCount, nullptr, GL_STATIC_READ);
	}

	//Lol this aint gonna work
	// GLuint placeholder = tb_current;
	// tb_current = tb_previous;
	// tb_previous = placeholder;
	return tb_current;
}

GLuint ParticleSystem::getPrevTBuf() {
	return tb_previous;
}