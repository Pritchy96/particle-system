// #include "stdafx.h"
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../include/renderable.hpp"

using namespace glm;
using namespace std;

GLuint Renderable::getVAO() {
	if (!validVAO) {
		cout << "Creating VAO for Renderable" << endl;

		vector<float> verts, cols;
		for (vector<glm::vec3>::const_iterator point = vertexes.begin(); point!=vertexes.end(); ++point) {
			verts.push_back(point->x);
			verts.push_back(point->y);
			verts.push_back(point->z); 
		}

		for (vector<glm::vec3>::const_iterator colour = colours.begin(); colour!=colours.end(); ++colour) {
			cols.push_back(colour->x);
			cols.push_back(colour->y);
			cols.push_back(colour->z); 

			cout << glm::to_string(*colour) << endl;
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glGenBuffers(1, &pos_vbo);
		glGenBuffers(1, &col_vbo);

		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(float), cols.data(), GL_STREAM_DRAW);

		validVAO = true;
	}

	return vao;
}

Renderable::Renderable(GLuint Shader) {
	shader = Shader;
}

//Uses Vert Data as placeholder Colour data too
Renderable::Renderable(GLuint Shader, vector<glm::vec3> vert_data) {
	shader = Shader;

	vertexes = vert_data;
	colours = vert_data;

}

Renderable::Renderable(GLuint Shader, vector<glm::vec3> vert_data, vector<glm::vec3> colour_data) {
	shader = Shader;

	vertexes = vert_data;
	colours = colour_data;

}