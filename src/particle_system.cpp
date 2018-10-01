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

Particle_System::Particle_System(vec3 Position, vec3 Colour, GLuint Shader, int numberOfParticles)
: Renderable(shader, vector<vec3>{Position}, vector<vec3>{Colour}) {
	particleCount = numberOfParticles;
}

GLuint Particle_System::getTransBuffer() {
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

GLuint Particle_System::getPrevTBuf() {
	return tb_previous;
}



// //Uses Vert Data as placeholder Colour data too
// Renderable::Renderable(GLuint Shader, vector<glm::vec3> vert_data) {
// 	shader = Shader;
// 	vertexes = vert_data;
// 	colours = vert_data;
// }

// Renderable::Renderable(GLuint Shader, vector<glm::vec3> vert_data, vector<glm::vec3> colour_data) {
// 	shader = Shader;
// 	vertexes = vert_data;
// 	colours = colour_data;
// }


