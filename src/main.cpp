#include <boost/filesystem.hpp>
#include <chrono>
#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "../include/render_environment.hpp"
#include "../include/shader.hpp"
#include "../include/particle_system.hpp"

using namespace std;
using namespace boost;

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

auto oldTime = chrono::steady_clock::now(), newTime = chrono::steady_clock::now();
double deltaT;
double sampleTime = 10000;
double currentSampleTime = sampleTime;
int maxParticleSystems = 100;
int maxParticles = 10000;
int particleSystemCount = 1;
GLuint particleRenderType = GL_POINTS;
double timeElapsed = 0;
int framesElapsed = 0;
std::ofstream file;


int main(int argc, const char* argv[]) {

	srand (time(NULL));
    cout << "Launching Program" << endl;

    renderEnvironment *renderer = new renderEnvironment();
    cout << "Initialised renderer" << endl;

	glfwSetWindowUserPointer(renderer->window, renderer);
	glfwSetKeyCallback(renderer->window, keyCallback);

    file.open ("test.csv");
	file << "Particle Count, 1 VAO\n";
	file << maxParticles;


	
    while (true) {  //TODO: Write proper update & exit logic.
		oldTime = newTime;
    	newTime = chrono::steady_clock::now();
		deltaT = chrono::duration_cast<chrono::milliseconds>(newTime - oldTime).count();




		currentSampleTime -= deltaT;

		timeElapsed += deltaT;
		framesElapsed++;

		if (currentSampleTime <= 0) {
			currentSampleTime = sampleTime;
			double fps = (double)(framesElapsed / timeElapsed) * 1000;
			char tmp[128];
			file << ", " << fps;
			framesElapsed = 0;
			timeElapsed = 0;

			if (particleSystemCount < maxParticleSystems) {
				particleSystemCount++;
			} else if (maxParticles < 1000000) {
				particleSystemCount = 1;
				maxParticles += 10000;
				file << "\n" << maxParticles;
			} else {
				file.close();
				exit(0);
			}			
		}
		
		
		for (int i = 0; i < particleSystemCount; i++) {	//Can spawn n emitters per update.
			if ((renderer->renderables.size() - 1) < particleSystemCount) {
				glm::vec3 origin = vec3( ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000);
				renderer->addRenderable(new ParticleSystem(renderer->particleShader, renderer->transformShader, origin, maxParticles/maxParticleSystems, particleRenderType));			
			}
		}

		//remove excess particle systems. Hacky, but fine for demo.
		if (renderer->renderables.size() > maxParticleSystems) {
			for (int i = 1; i < (renderer->renderables.size() - maxParticleSystems); i++) {
				renderer->renderables[i]->isDead = true;
			}
		}



		

        renderer->update(deltaT);
    }


	file.close();
    return 0;
}


static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	renderEnvironment *rEnv = (renderEnvironment *)glfwGetWindowUserPointer(window);

	switch (key) {
		case(GLFW_KEY_ESCAPE) :
			file.close();
			exit(0);
			break;

		case(GLFW_KEY_V) :
			if (action == GLFW_PRESS) {
				GLint mode[2];
				glGetIntegerv(GL_POLYGON_MODE, mode);
				if (mode[0] == GL_LINE) {
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					fprintf(stdout, "Switching View to GL_FILL.\n" );
				} else {
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					fprintf(stdout, "Switching View to GL_LINE.\n" );
				}
			}
			break;
		case(GLFW_KEY_EQUAL) :
			maxParticleSystems++;	
			break;
		case(GLFW_KEY_MINUS) :
			maxParticleSystems--;	
			break;
		case(GLFW_KEY_1) :
			particleRenderType = GL_POINTS;	
			break;
		case(GLFW_KEY_2) :
			particleRenderType = GL_LINES;	
			break;
		case(GLFW_KEY_3) :
			particleRenderType = GL_TRIANGLES;	
			break;
		default:
			break;
	}
}