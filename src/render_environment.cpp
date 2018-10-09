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

double timeElapsed = 0;
int framesElapsed = 0;

Renderable* renderAxis;

GLuint basicShader;

vector<vec3> axis_lines = {
    vec3(0.0f, 0.0f, 0.0f),
	vec3(100.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 100.0f, 0.0f),    
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 100.0f)
};

vector<vec3> axis_colours = {
    vec3(1.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),    
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f)
};

void renderEnvironment::setFPSCounter(GLFWwindow* window, double deltaT) {
	timeElapsed += deltaT;
	framesElapsed++;

	//If it's more than a quarter of a second, update fps.
	if (timeElapsed > 250) {
		double fps = (double)(framesElapsed / timeElapsed) * 1000;
		char tmp[128];
		//Write formatted data to tmp string.
		snprintf(tmp, sizeof(tmp)/sizeof(char), "Particle System @ %.2f FPS", fps);
		//Set window title to string.
		glfwSetWindowTitle(window, tmp);
		framesElapsed = 0;
		timeElapsed = 0;
	}
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

	glClearColor(0.00f, 0.00f, 0.00f, 0.01f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(2);
	glLineWidth(4); //This doesn't work?

	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
	}
	
	basicShader = Shader::LoadShaders("./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader");
    renderAxis = new Renderable(basicShader, axis_lines, axis_colours);
	addRenderable(renderAxis);

}

void renderEnvironment::addRenderable(Renderable* renderable) {
	renderables.push_back(renderable);
}


void renderEnvironment::setupTransformShader(GLuint transformShader) {
	tShader = transformShader;
}

void renderEnvironment::update(float deltaT) {
	input->update(window);
	setFPSCounter(window, deltaT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vector<Renderable*>::iterator renderable = renderables.begin();

	while(renderable != renderables.end()) {
		if((*renderable)->isDead) {
			delete(*renderable);
			//iterator.erase gives the next item in the list.
			renderable = renderables.erase(renderable);
		} else {
			(*renderable)->Draw(deltaT, input->getProjectionMatrix(), input->getViewMatrix());
			++renderable;
		}
	}

	//cout << endl;

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