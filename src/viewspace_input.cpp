// #include "stdafx.h"
#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../include/viewspace_input.hpp"

using namespace glm;
using namespace std;

vec3 cameraPos, cameraPosDefault;
mat4 viewMatrix, projectionMatrix;
float horizontalCameraAngle = 3.14f, verticalCameraAngle = 1.0f, initialCameraFOV = 45.0f;
int mouseXMax, mouseYMax;
double mouseXPos, mouseYPos;
float mouseCoef = 0.001f;
float translationSpeed = 300.0f; // 3 units / second
double lastFrameTime;
double currentFrameTime;

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
}

void viewspaceManipulator::update(GLFWwindow *window){
	currentFrameTime = glfwGetTime();
	float deltaTime = float(currentFrameTime - lastFrameTime);

	// Get mouse cameraPos
	glfwGetCursorPos(window, &mouseXPos, &mouseYPos);

	//Get window size.
	//TODO: Shouldn't do this every update loop! Only when it changes.
	glfwGetWindowSize(window, &mouseXMax, &mouseYMax);

	// recenter mouse cameraPos for next frame
	glfwSetCursorPos(window, mouseXMax / 2, mouseYMax / 2);

	// Compute new orientation
	horizontalCameraAngle += mouseCoef * float(mouseXMax / 2 - mouseXPos);
	verticalCameraAngle += mouseCoef * float(mouseYMax / 2 - mouseYPos);

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	vec3 direction(
		cos(verticalCameraAngle) * sin(horizontalCameraAngle),
		sin(verticalCameraAngle),
		cos(verticalCameraAngle) * cos(horizontalCameraAngle)
		);

	// Right vector
	vec3 right = vec3(
		sin(horizontalCameraAngle - 3.14f / 2.0f),
		sin(horizontalCameraAngle),
		cos(horizontalCameraAngle - 3.14f / 2.0f)
		);

	// Up vector
	vec3 up = cross(right, direction);

	float currentTranslationSpeed = translationSpeed;

	// Speed modifier
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		currentTranslationSpeed *= 2;
	}

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cameraPos += direction * deltaTime * currentTranslationSpeed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cameraPos -= direction * deltaTime * currentTranslationSpeed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cameraPos += right * deltaTime * currentTranslationSpeed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cameraPos -= right * deltaTime * currentTranslationSpeed;
	}

	// Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		cameraPos += up * deltaTime * currentTranslationSpeed;
	}

	// Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		cameraPos -= up * deltaTime * currentTranslationSpeed;
	}

	float FoV = initialCameraFOV;

	// Projection matrix : 45 Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	projectionMatrix = perspective(FoV, 4.0f / 3.0f, 0.1f, 100000000.0f);
	// Camera matrix
	viewMatrix = lookAt(
		cameraPos,           // Camera is here
		cameraPos + direction, // and looks here : at the same cameraPos, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
		);

	// For the next frame, the "last time" will be "now"
	lastFrameTime = currentFrameTime;
}
	
