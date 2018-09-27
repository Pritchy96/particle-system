// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/shader.hpp"
#include "../include/viewspace_manipulator.hpp"

using namespace std;
using namespace glm;

GLFWwindow* window;
int gl_width = 1024;
int gl_height = 768;

viewspaceManipulator* input;
double rotate_y = 0; 
double rotate_x = 0;
double rotate_z = 0;
double scale = 0.01;

vector<float> test_data_cube = {
    -1.0f, -1.0f, -1.0f, // triangle 1 : begin
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f, -1.0f, // triangle 2 : begin
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f, // triangle 2 : end...
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

vector<float> test_data_lines = {
    -1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, 1.0f
};

static void error_callback(int error, const char* description) {
	fputs(description, stderr);	//Error logging stuff
}

static void window_size_callback(GLFWwindow* window, int width, int height) {
	gl_height = height;
	gl_width = width;
	glViewport(0, 0, gl_width, gl_height);
}

//Blatantly stolen.
void update_fps_counter(GLFWwindow* window) {
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

int main( void ) {
	if( !glfwInit() ) {
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwSetErrorCallback(error_callback);

	input = new viewspaceManipulator(window);

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(gl_width, gl_height, "Test Project", NULL, NULL);

	if( window == NULL ) {
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, input->key_callback);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwMakeContextCurrent(window);

	glClearColor(0.7f, 0.7f, 0.7f, 0.0f);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	GLuint basicShader = LoadShaders( "./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader" );
	GLuint MatrixID = glGetUniformLocation(basicShader, "MVP"); //Get a handle for the MVP uniform var from the shader program

	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, test_data_lines.size() * sizeof(float), test_data_lines.data(), GL_STATIC_DRAW);

	GLuint col_vbo = 0;
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	//NOTE: Here we are using the same data for position as well as colour.
	glBufferData(GL_ARRAY_BUFFER, test_data_lines.size() * sizeof(float), test_data_lines.data(), GL_STATIC_DRAW);

	//A Vertex Attribute Object (VAO) remembers all of the Vertex Buffer Objects (VBO's) that you want to use, and the memory layout of each one. 
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	//Defining layout of our first VBO within the VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	while(!glfwWindowShouldClose(window)) {
		input->update(window);

		//Model matrix : an identity matrix (model will be at the origin)
		//Just giving mat4 one param will make it an identity matrix
		//This means that the coords (of the model) will be zero, as they are the top four rows of the last column
		glm::mat4 Model = glm::mat4(1.0f);  // Changes for each model
		//Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP = input->getProjectionMatrix() * input->getViewMatrix() * Model;

		//Send our transformation to the currently bound shader, in the "MVP" uniform
		//For each model you render, since the MVP will be different (at least the M part)
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		update_fps_counter(window);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(basicShader);

		//1st attribute buffer : vertices
		glBindVertexArray(vao);
		glDrawArrays(GL_LINES, 0, test_data_lines.size() / 3);

		//Put the stuff we've been drawing onto the display
		glfwSwapBuffers(window);

		//Update other events like input handling 
		glfwPollEvents();
	}
	
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
	return 0;
}