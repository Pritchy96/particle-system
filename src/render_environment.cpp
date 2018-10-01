#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>


#include "../include/shader.hpp"
#include "../include/render_environment.hpp"
#include "../include/renderable.hpp"
#include "../include/particle_system.hpp"
#include "../include/viewspace_input.hpp"

using namespace std;
using namespace glm;

GLFWwindow* window;
viewspaceManipulator* input;

GLuint shaderID;

int gl_width = 1024;
int gl_height = 768;

vec3 initialCameraPos = glm::vec3(10, 10, 10);
double rotate_y = 0; 
double rotate_x = 0;
double rotate_z = 0;

glm::mat4 scaleMatrix = glm::scale(glm::vec3(10.0, 10.0, 10.0));

//Blatantly stolen.
void renderEnvironment::update_fps_counter(GLFWwindow* window) {
	//Get time.
	static double previous_seconds = glfwGetTime();
	static int frame_count;
	//Get time again.
	double current_seconds = glfwGetTime();
	//Get time difference.
	double elapsed_seconds = current_seconds - previous_seconds;

	//If it's more than a quarter of a second, update fps. 
	//This is to stop it jumping around too much by averaging the fps over 0.25 of a second.
	if (elapsed_seconds > 0.25) {
		previous_seconds = current_seconds;
		double fps = (double)frame_count / elapsed_seconds;
		char tmp[128];
		//Write formatted data to tmp string.
		// sprintf_s(tmp, "opengl renderer @ fps: %.2f", fps);
		snprintf(tmp, sizeof(tmp)/sizeof(char), "opengl renderer @ fps: %.2f", fps);
		//Set window title to string.
		glfwSetWindowTitle(window, tmp);
		frame_count = 0;
	}
	frame_count++;
}

renderEnvironment::renderEnvironment() {
	//Our ModelViewProjection : multiplication of our 3 matrices
	if( !glfwInit() ) {
		// glDrawArrays(GL_POINTS, 0, particle_system
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwSetErrorCallback(error_callback);

	input = new viewspaceManipulator(window, initialCameraPos);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		// glDrawArrays(GL_POINTS, 0, particle_system
	
	window = glfwCreateWindow(gl_width, gl_height, "Particle System", NULL, NULL);
		// glDrawArrays(GL_POINTS, 0, particle_system

	if( !window ) {
		fprintf(stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
	}	

	glfwSetKeyCallback(window, input->key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(4);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}
}

void renderEnvironment::addRenderable(Renderable renderable) {
	renderables.push_back(renderable);
}

void renderEnvironment::addParticleSystem(ParticleSystem particle_system) {
	particle_systems.push_back(particle_system);
}

void renderEnvironment::setupTransformShader(GLuint transformShader) {
	tShader = transformShader;
}

void renderEnvironment::update() {
	input->update(window);
	update_fps_counter(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (vector<ParticleSystem>::iterator particle_system = particle_systems.begin(); particle_system!=particle_systems.end(); ++particle_system) {
		glm::mat4 MVP = input->getProjectionMatrix() * input->getViewMatrix() * particle_system->modelMatrix;

		shaderID = glGetUniformLocation(particle_system->shader, "scale");
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &scaleMatrix[0][0]);
		
		shaderID = glGetUniformLocation(particle_system->shader, "MVP"); 
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &MVP[0][0]);
		
		//Turn rendering off
		glEnable(GL_RASTERIZER_DISCARD);
		
		glUseProgram(tShader);
		glBindVertexArray(particle_system->getVAO());
		GLuint tbuf = particle_system->getTransBuffer();

		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbuf);
		glBeginTransformFeedback(GL_POINTS);

		if(particle_system->isNewSystem) {
			glDrawArrays(GL_POINTS, 0, particle_system->particleCount);
			particle_system->isNewSystem = false;
		} else { 
			 glDrawArrays(GL_POINTS, 0, particle_system->particleCount);

		 	// glDrawTransformFeedback(GL_POINTS, particle_system->getPrevTBuf());
		}

		glEndTransformFeedback();
		// glBindBuffer(GL_ARRAY_BUFFER,0);

		GLfloat feedback[particle_system->particleCount*4];
		glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

		int k=0;
        for(int i=0; i < particle_system->particleCount*4; i++) {            
            cout<<feedback[i]<<" ";
            k++;
            if(k==4){cout<<endl; k=0;}
        }

		cout << endl;

		// //Turn rendering ON
        glDisable(GL_RASTERIZER_DISCARD);

		//Render particles from feedback object Current

		glUseProgram(particle_system->shader);

		// glDrawTransformFeedback(GL_POINTS, tbuf);
		glDrawArrays(GL_POINTS, 0, particle_system->particleCount);
	}

	glfwSwapBuffers(window);

	//Update other events like input handling 
	glfwPollEvents();
}

renderEnvironment::~renderEnvironment() {
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void renderEnvironment::error_callback(int error, const char* description) {
	fputs(description, stderr);	//Error logging stuff
}

void renderEnvironment::window_size_callback(GLFWwindow* window, int width, int height) {
	gl_height = height;
	gl_width = width;
	glViewport(0, 0, gl_width, gl_height);
}