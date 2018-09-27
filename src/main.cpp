#include <iostream>

#include <boost/filesystem.hpp>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL //gtx = gt eXperimental?
#include <glm/gtx/string_cast.hpp>

#include "../include/render_environment.hpp"
#include "../include/shader.hpp"

using namespace std;
using namespace boost;

vector<vec3> axis_lines = {
    vec3(0.0f, 0.0f, 0.0f),
	vec3(1000.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 1000.0f, 0.0f),    
	vec3(0.0f, 0.0f, 0.0f),
	vec3(0.0f, 0.0f, 1000.0f)
};

vector<vec3> axis_colours = {
    vec3(1.0f, 0.0f, 0.0f),
	vec3(1.0f, 0.0f, 0.0f),
	vec3(0.0f, 1.0f, 0.0f),    
	vec3(0.0f, 1.0f, 0.0f),
	vec3(0.0f, 0.0f, 1.0f),
	vec3(0.0f, 0.0f, 1.0f)
};


int main(int argc, const char* argv[]) {
    cout << "Launching Program" << endl;

    renderEnvironment *renderer = new renderEnvironment();
    cout << "Initialised renderer" << endl;

	GLuint basicShader = LoadShaders("./bin/shaders/basic.vertshader", "./bin/shaders/basic.fragshader");

    renderer->addRenderable(*new Renderable(basicShader, axis_lines, axis_colours));

    while (true) {  //TODO: Write proper update&exit logic.
        renderer->update();
    }

    return 0;
}