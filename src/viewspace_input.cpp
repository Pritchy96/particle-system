// #include "stdafx.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../include/viewspace_input.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include "../include/arcball.h"

using namespace glm;
using namespace std;

vec3 cameraPos, cameraPosDefault;
mat4 viewMatrix, projectionMatrix, defaultView, rotationMatrix;
float horizontalMouseDelta = 3.14f, verticalMouseDelta = 1.0f, initialCameraFOV = 45.0f;
int windowWidth, windowHeight;
float mouseCoef = 0.001f;
float translationSpeed = 300.0f; 
double lastFrameTime, mouseXLastFrame, mouseYLastFrame;
double currentFrameTime;
double yaw = 0.0, pitch = 0.0, zoom = 100.0f;
Arcball* arcball;
float cameraDistance = 30.0f;


mat4 viewspaceManipulator::getViewMatrix() {
	return viewMatrix;
}

mat4 viewspaceManipulator::getProjectionMatrix() {
	return projectionMatrix;
}

viewspaceManipulator::viewspaceManipulator(GLFWwindow *window, vec3 initialCameraPos) {
	// glfwSetKeyCallback(window, viewspaceManipulator::key_callback);
		currentFrameTime = glfwGetTime();
		lastFrameTime = glfwGetTime();
		cameraPosDefault = cameraPos = initialCameraPos;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		arcball = new Arcball( windowWidth, windowHeight, .015f, true, true );

		float FoV = initialCameraFOV;

		// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
		projectionMatrix = glm::perspective(70.0f, 4.0f/3.0f, 0.1f, 10000000.0f);
		rotationMatrix = glm::mat4(1.0);
}

void viewspaceManipulator::update(GLFWwindow *window){
	currentFrameTime = glfwGetTime();
	float deltaTime = float(currentFrameTime - lastFrameTime);

	// if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		arcball->update(deltaTime);
		
		// Camera matrix
		rotationMatrix *= arcball->createViewRotationMatrix();
		viewMatrix = lookAt(glm::vec3(0.0f, 0.0f, cameraDistance), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0)) * rotationMatrix;

		
	// }

	// For the next frame, the "last time" will be "now"
	lastFrameTime = currentFrameTime;
}
void viewspaceManipulator::mouseButtonCallback( GLFWwindow* window, int button, int action, int mods ){
    arcball->mouseButtonCallback( window, button, action, mods );
}
 
void viewspaceManipulator::cursorCallback( GLFWwindow *window, double x, double y ) {
    arcball->cursorCallback(window, x, y);
}

void viewspaceManipulator::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	cameraDistance = std::max(1.0, cameraDistance+yoffset);
}
	
void viewspaceManipulator::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case(GLFW_KEY_ESCAPE) :
		exit(0);
		break;

	case(GLFW_KEY_V) :
		if (action == GLFW_PRESS) {
			GLint mode[2];
			glGetIntegerv(GL_POLYGON_MODE, mode);
			if (mode[0] == GL_LINE) {
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				fprintf( stdout, "Switching View to GL_FILL.\n" );
			} else {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				fprintf( stdout, "Switching View to GL_LINE.\n" );
			}
		}
		break;
	default:
		break;
	} 
}