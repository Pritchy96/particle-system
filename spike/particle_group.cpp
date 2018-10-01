// #include "stdafx.h"
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/particle_group.hpp"

using namespace glm;
using namespace std;

Particle_Group::Particle_Group(GLuint Shader, glm::vec3 Colour, glm::vec3 Position, glm::vec3 Acceleration, glm::vec3 Velocity, int numberOfParticles, float Lifespan, float particleSize) 
: Renderable(shader, vector<vec3>{Position}, vector<vec3>{Colour}) {

}

Particle_Group::Particle_Group(GLuint Shader, glm::vec3 Colour, glm::vec3 Position, int numberOfParticles) 
: Renderable(shader) {
	vector<glm::vec3> verts, cols;

	for (int i = 0; i < numberOfParticles; i++) {
		vertexes.push_back(Position);
		colours.push_back(Colour);
	}
}
	
GLuint Particle_Group::getVAO() {
	if (!validVAO) {
		generateVAO();
	}

	return vao;
}

void Particle_Group::generateVAO() {

	Renderable::generateVAO();
	cout << "Creating VAO for Particle Group" << endl;
	
	
	vector<float> velocities, cols;
	// for (vector<glm::vec3>::const_iterator point = vertexes.begin(); point!=vertexes.end(); ++point) {
	// 	verts.push_back(point->x);
	// 	verts.push_back(point->y);
	// 	verts.push_back(point->z); 
	// }

	// for (vector<glm::vec3>::const_iterator colour = vertexes.begin(); colour!=vertexes.end(); ++colour) {
	// 	cols.push_back(colour->x);
	// 	cols.push_back(colour->y);
	// 	cols.push_back(colour->z); 
	// }

	// glGenVertexArrays(1, &vao);
	// glBindVertexArray(vao);
	// glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);
	// glGenBuffers(1, &pos_vbo);
	// glGenBuffers(1, &col_vbo);

	// glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STREAM_DRAW);

	// glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	// glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(float), cols.data(), GL_STREAM_DRAW);
	

	// validVAO = true;
}

