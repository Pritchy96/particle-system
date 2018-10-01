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
		fprintf( stderr, "Failed to initialize GLFW\n" );
	}

	glfwSetErrorCallback(error_callback);

	input = new viewspaceManipulator(window, initialCameraPos);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	window = glfwCreateWindow(gl_width, gl_height, "Particle System", NULL, NULL);

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

void renderEnvironment::update() {
	input->update(window);
	update_fps_counter(window);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (vector<Renderable>::iterator renderable = renderables.begin(); renderable!=renderables.end(); ++renderable) {
		glm::mat4 MVP = input->getProjectionMatrix() * input->getViewMatrix() * renderable->modelMatrix;

		shaderID = glGetUniformLocation(renderable->shader, "scale");
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &scaleMatrix[0][0]);
		
		shaderID = glGetUniformLocation(renderable->shader, "MVP"); 
		glUniformMatrix4fv(shaderID, 1, GL_FALSE, &MVP[0][0]);

		glUseProgram(renderable->shader);
		glBindVertexArray(renderable->getVAO());
		glDrawArrays(GL_POINTS, 0, renderable->vertexes.size());
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