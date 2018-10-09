// #include "stdafx.h"
#include <iostream>
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/random.hpp>

#include "../include/particle_system.hpp"

using namespace glm;
using namespace std;

ParticleSystem::ParticleSystem(GLuint Shader, GLuint TransformShader, glm::vec3 origin, int numberOfParticles)
: Renderable(Shader) {

	transformShader = TransformShader;
	particleCount = numberOfParticles;
	age = 40.0f;

    modelMatrix = glm::translate(mat4(1.0f), origin);

	vec3 baseCol = vec3((float) rand() / RAND_MAX, (float) rand() / RAND_MAX, (float) rand() / RAND_MAX);

    for (int i = 0; i < numberOfParticles; i++) {
        vertexes.push_back(vec3(0.0f)); //Init all new particles at emitter origin.
        colours.push_back(baseCol + vec3(((float) rand() / RAND_MAX) / 4 - 0.25, ((float) rand() / RAND_MAX) / 4 - 0.25, ((float) rand() / RAND_MAX) / 4 - 0.25));
    }
}

void ParticleSystem::Draw(float deltaT, glm::mat4 projectionMatrix, glm::mat4 viewMatrix) {

		//cout << "Drawing Particle System" << endl;

		glUseProgram(transformShader);

		GLuint current_vao = getVAO();
		glBindVertexArray(current_vao);

		GLuint shaderID = glGetUniformLocation(transformShader, "deltaT"); 
		glUniform1f(shaderID, deltaT);

		shaderID = glGetUniformLocation(transformShader, "wind");
		glUniform3fv(shaderID, 1, &vec3(0.02f, 0.01f, 0.0f)[0]);
		

		glEnable(GL_RASTERIZER_DISCARD);	//Turn rendering off
	
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, pos2_vbo);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 1, vel2_vbo);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 2, age2_vbo);

		glBeginTransformFeedback(GL_POINTS);
		glDrawArrays(GL_POINTS, 0, vertexes.size());
		glEndTransformFeedback();
 		glBindBuffer(GL_ARRAY_BUFFER,0);	//Unbind buffer (best practice)

		// //Turn rendering ON
        glDisable(GL_RASTERIZER_DISCARD);

		// int feedbackLength = particleCount*3*2;
		// GLfloat feedback[feedbackLength];
		// glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

		// int k=0;
        // for(int i=0; i < feedbackLength; i++) {            
        //     cout<<feedback[i] << " ";
        //     k++;
        //     if(k==3) {
		// 		cout<<endl; k=0;   
		// 		}
		// }
		// cout << endl;

 		glBindVertexArray(0);	//Unbind buffers (best practice)

		glBindVertexArray(getPrevVAO());	//Draw from the other VAO.

		glUseProgram(shader);

		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;

		shaderID = glGetUniformLocation(shader, "scale");
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &scaleMatrix[0][0]);
		
		shaderID = glGetUniformLocation(shader, "MVP"); 
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &MVP[0][0]);

		// glDrawTransformFeedback(GL_POINTS, tbuf);
		glDrawArrays(GL_POINTS, 0, particleCount);

		age -= 0.1f;
		if (age <= 0.0f) {
			isDead = true;
		}
}

GLuint ParticleSystem::getVAO() {
	if (!validVAO) {
		//cout << "Creating 2 VAOs for Particle System, will call getVAO() for Renderable to set up first VAO:" << endl; 
		//Setup base VAO, with additional Particle system only parameters.
		GLint vao = Renderable::getVAO();
		glBindVertexArray(vao);

		vector<float> verts, cols, vels, ages;

		for (int i = 0; i < particleCount; i++) {
			cols.push_back(colours[i].x);
			cols.push_back(colours[i].y);
			cols.push_back(colours[i].z);

			verts.push_back(vertexes[i].x);
			verts.push_back(vertexes[i].y);
			verts.push_back(vertexes[i].z);
			
			vec3 vel = glm::sphericalRand(4 + ((double) rand() / (RAND_MAX) / 5));
			vels.push_back(vel.x);
			vels.push_back(vel.y);
			vels.push_back(vel.z);

			//Slightly randomise particle timouts
			ages.push_back((age/100.0f)-((float) rand() / RAND_MAX));
		}

		glGenBuffers(1, &pos2_vbo);
		glGenBuffers(1, &vel_vbo);
		glGenBuffers(1, &vel2_vbo);
		glGenBuffers(1, &age_vbo);
		glGenBuffers(1, &age2_vbo);

		glBindBuffer(GL_ARRAY_BUFFER,pos2_vbo);	//Bind and allocate pos2 buffer.
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER,pos_vbo);	//Rebind pos1 buffer.

		glBindBuffer(GL_ARRAY_BUFFER, vel_vbo);
		glEnableVertexAttribArray(2);
		glBufferData(GL_ARRAY_BUFFER, vels.size() * sizeof(float), vels.data(), GL_STATIC_COPY);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vel2_vbo);	//Bind and allocate vel2 buffer.
		glBufferData(GL_ARRAY_BUFFER, vels.size() * sizeof(float), vels.data(), GL_STATIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, vel_vbo);	//Rebind vel1 buffer.

		glBindBuffer(GL_ARRAY_BUFFER, age_vbo);
		glEnableVertexAttribArray(3);
		glBufferData(GL_ARRAY_BUFFER, ages.size() * sizeof(float), ages.data(), GL_STATIC_COPY);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, age2_vbo);	//Bind and allocate age2 buffer.
		glBufferData(GL_ARRAY_BUFFER, ages.size() * sizeof(float), ages.data(), GL_STATIC_COPY);
		glBindBuffer(GL_ARRAY_BUFFER, age_vbo);	//Rebind age1 buffer.

		glBindVertexArray(0);
		
		// Now set up a second VAO for double buffering with Transform Feedback.
		glGenVertexArrays(1, &vao2);
		glBindVertexArray(vao2);

		glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, vel_vbo);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindBuffer(GL_ARRAY_BUFFER, age_vbo);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindVertexArray(0);
	}
	
	std::swap(pos_vbo, pos2_vbo);
	std::swap(vel_vbo, vel2_vbo);
	std::swap(age_vbo, age2_vbo);

	return vao;
}

GLuint ParticleSystem::getPrevVAO() {
	//This returns the VAO which we don't want to write to with Transform Feedback, i.e the one we're drawing from.
	return vao2;
}

ParticleSystem::~ParticleSystem() {
	// cout << "Deleting Particle System" << endl;
	//I'm convinced this is definitely by far the best way of doing this (/s)
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &pos2_vbo);
	glDeleteBuffers(1, &col_vbo);
	glDeleteBuffers(1, &col2_vbo);
	glDeleteBuffers(1, &age_vbo);
	glDeleteBuffers(1, &age2_vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteVertexArrays(1, &vao2);
}
