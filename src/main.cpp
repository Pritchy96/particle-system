#include <iostream>
#include <chrono>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL //gtx = gt eXperimental?
#include <glm/gtx/string_cast.hpp>

#include "../include/render_environment.hpp"
#include "../include/shader.hpp"
#include "../include/particle_system.hpp"


using namespace std;
using namespace boost;

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

vector<vec3> test_data_lines = {
	vec3(0.000000, 4.000000, 12.000000),
	vec3(2.870316, -101.411970, 1.000000),

	vec3(0.000000, 0.000000, 0.2233231),
	vec3(3.897837, 14.548908, 1.000000),

	vec3(4.398729, 18.116240, 2.213213),
	vec3(5.055743, 6.247947, 1.000000),

	vec3(5.365756, 6.035206, 4.000000),
	vec3(5.693254, 14.517317, 1.00000)
};

auto oldTime = chrono::steady_clock::now(), newTime = chrono::steady_clock::now();
double deltaT;

int main(int argc, const char* argv[]) {

	srand (time(NULL));
    cout << "Launching Program" << endl;

    renderEnvironment *renderer = new renderEnvironment();
    cout << "Initialised renderer" << endl;

	GLuint basicShader = Shader::LoadShaders("./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader");
	GLuint particleShader = Shader::LoadShaders("./bin/shaders/particle.vertshader", "./bin/shaders/basic.fragshader");
	GLuint transformShader = Shader::LoadTransformShader("./bin/shaders/transform.vertshader");

    // renderer->addRenderable(new Renderable(basicShader, axis_lines, axis_colours));
	



	// for (int i = 0; i < 1500; i++) {
	// 	glm::vec3 origin = vec3( ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000);
	// 	renderer->addRenderable(new ParticleSystem(particleShader, transformShader, origin, 400));
	// }
	
    while (true) {  //TODO: Write proper update & exit logic.
		oldTime = newTime;
    	newTime = chrono::steady_clock::now();
		deltaT = chrono::duration_cast<chrono::milliseconds>(newTime - oldTime).count();

		for (int i = 0; i < 2; i++) {	//Can spawn two emitters per update.
			if (renderer->renderables.size() < 1500) {
				glm::vec3 origin = vec3( ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000, ((float) rand() / RAND_MAX) * 1000);
				renderer->addRenderable(new ParticleSystem(particleShader, transformShader, origin, 400));			
			}
		}

		cout << "delta: " << deltaT << endl;
        renderer->update();
    }

    return 0;
}