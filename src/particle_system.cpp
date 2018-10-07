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

ParticleSystem::ParticleSystem(GLuint Shader, GLuint TransformShader, glm::vec3 origin, int numberOfParticles)
: Renderable(Shader) {

	transformShader = TransformShader;
	particleCount = numberOfParticles;

    modelMatrix = glm::translate(mat4(1.0f), origin);

    for (int i = 0; i < numberOfParticles; i++) {
        vertexes.push_back(vec3(0.0f)); //Init all new particles at emitter origin.
        colours.push_back(vec3(0.4f, 1.0f, 0.3f));
    }
}

void ParticleSystem::Draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {
		cout << "Drawing Particle System" << endl;

		glUseProgram(transformShader);

		GLuint current_vao = getVAO();
		glBindVertexArray(current_vao);

		//Turn rendering off
		glEnable(GL_RASTERIZER_DISCARD);
	
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, pos2_vbo);



		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, vertexes.size());
		glEndTransformFeedback();
 		glBindBuffer(GL_ARRAY_BUFFER,0);
    	glFlush();

		// //Turn rendering ON
        glEnable(GL_RASTERIZER_DISCARD);

		std::swap(pos_vbo, pos2_vbo);


		GLfloat feedback[particleCount*3];
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

		int k=0;
        for(int i=0; i < particleCount*3; i++) {            
            cout<<feedback[i] << " ";
            k++;
            if(k==3) {cout<<endl; k=0;}        }

		cout << endl;

 		glBindVertexArray(0);

		// glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		// GLuint shaderID = glGetUniformLocation(shader, "scale");
		// glUniformMatrix4fv(shaderID, 1, GL_FALSE, &scaleMatrix[0][0]);
		
		// shaderID = glGetUniformLocation(shader, "MVP"); 
		// glUniformMatrix4fv(shaderID, 1, GL_FALSE, &MVP[0][0]);

		// glBindVertexArray(getPrevVAO());
	
		// //Render particles from feedback object Current
		// glUseProgram(shader);

		// // glDrawTransformFeedback(GL_POINTS, tbuf);
		// glDrawArrays(GL_POINTS, 0, particleCount);
}

GLuint ParticleSystem::getVAO() {
	if (!validVAO) {
		cout << "Creating 2 VAOs for Particle System, will call getVAO() for Renderable:" << endl; 
		//Setup base VAO, with additional Particle system only parameters.
		GLint vao = Renderable::getVAO();
		glBindVertexArray(vao);

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
		}

		glGenBuffers(1, &pos2_vbo);
		glBindBuffer(GL_ARRAY_BUFFER,pos2_vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);

		glBindVertexArray(0);
		
		// glEnableVertexAttribArray(2);
		// glGenBuffers(1, &vel_vbo);
		// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STREAM_DRAW);

		// Now set up a second VAO for double buffering with Transform Feedback.
		glGenVertexArrays(1, &vao2);
		glBindVertexArray(vao2);

		glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(0);




		// glEnableVertexAttribArray(0);
		// glEnableVertexAttribArray(1);
		// glEnableVertexAttribArray(2);
		// glEnableVertexAttribArray(1);
		// glEnableVertexAttribArray(2);

		// glGenBuffers(1, &pos2_vbo);
		// // glGenBuffers(1, &col2_vbo);

		// glBindBuffer(GL_ARRAY_BUFFER,pos2_vbo);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexes.size() * 3, verts.data(), GL_STATIC_READ);

		// glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		
		// glGenBuffers(1, &vel2_vbo);

		// glBindBuffer(GL_ARRAY_BUFFER, pos2_vbo);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_READ);

		// glBindBuffer(GL_ARRAY_BUFFER, col2_vbo);
		// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// glBufferData(GL_ARRAY_BUFFER, cols.size() * sizeof(float), cols.data(), GL_STATIC_READ);

		// glBindBuffer(GL_ARRAY_BUFFER, vel2_vbo);
		// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
		// std::swap(vao, vao2);
		// std::swap(col_vbo, col2_vbo);
		// std::swap(vel_vbo, vel2_vbo);
	
	return vao;
}

GLuint ParticleSystem::getPrevVAO() {
	//This returns the VAO which we don't want to write to with Transform Feedback, i.e the one we're drawing from.
	return vao2;
}