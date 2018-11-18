#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include "../include/shader.hpp"

using namespace std;

GLuint Shader::LoadShaders(char* vertFilePath, char* fragFilePath){
	cout << "Compiling Shaders" << endl;

	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);


	string vertShaderCode = LoadFileFromDisk(vertFilePath);
	string fragShaderCode = LoadFileFromDisk(fragFilePath);
	cout << "Shaders Read from File" << endl;

	GLint result = GL_FALSE; int infoLogSize;

	// Compile Vertex Shader
	printf("Compiling shader: %s\n", vertFilePath);
	char const * vertCodePntr = vertShaderCode.c_str();
	glShaderSource(vertShaderID, 1, &vertCodePntr , NULL);
	glCompileShader(vertShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogSize);
	vector<char> VertexShaderErrorMessage(infoLogSize);
	glGetShaderInfoLog(vertShaderID, infoLogSize, NULL, &VertexShaderErrorMessage[0]);
	
	if (&VertexShaderErrorMessage[0]) {
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
	cout << "Vertex Shader Compiled and Checked" << endl;


	// Compile Fragment Shader
	printf("Compiling shader: %s\n", fragFilePath);
	char const * FragmentSourcePointer = fragShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogSize);
	vector<char> FragmentShaderErrorMessage(infoLogSize);
	glGetShaderInfoLog(FragmentShaderID, infoLogSize, NULL, &FragmentShaderErrorMessage[0]);

	if (&FragmentShaderErrorMessage[0]) {
		fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
	}
	cout << "Fragment Shader Compiled and Checked" << endl;

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertShaderID);
	glAttachShader(ProgramID, FragmentShaderID);

	glLinkProgram(ProgramID); 
	glUseProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogSize);
	vector<char> ProgramErrorMessage( max(infoLogSize, int(1)) );
	glGetProgramInfoLog(ProgramID, infoLogSize, NULL, &ProgramErrorMessage[0]);
	
	if (&ProgramErrorMessage[0]) {
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vertShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

string Shader::LoadFileFromDisk(char* path) {
	string contents;
	ifstream fileStream(path, ios::in);
	if(fileStream.is_open()){
		string line = "";
		while(getline(fileStream, line))
			contents += "\n" + line;
		fileStream.close();
	}
	return contents;
}

GLuint Shader::LoadTransformShader(char* path){
	cout << "Compiling Shaders" << endl;

	GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);

	string vertShaderCode = LoadFileFromDisk(path);
	cout << "Shader Read from File" << endl;

	GLint result = GL_FALSE; int infoLogSize;

	// Compile Vertex Shader
	printf("Compiling shader: %s\n", path);
	char const * vertCodePntr = vertShaderCode.c_str();
	glShaderSource(vertShaderID, 1, &vertCodePntr , NULL);
	glCompileShader(vertShaderID);

	// Check Vertex Shader
	glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &infoLogSize);
	vector<char> VertexShaderErrorMessage(infoLogSize);
	glGetShaderInfoLog(vertShaderID, infoLogSize, NULL, &VertexShaderErrorMessage[0]);
	
	if (&VertexShaderErrorMessage[0]) {
		fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);
	}
	cout << "Vertex Shader Compiled and Checked" << endl;

	// Link the program
	fprintf(stdout, "Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertShaderID);

	const GLchar* Varyings[4];
    Varyings[0] = "out_pos";
    Varyings[1] = "out_vel";
    Varyings[2] = "out_age";

	glTransformFeedbackVaryings(ProgramID, 3, Varyings, GL_SEPARATE_ATTRIBS);
    // glTransformFeedbackVaryings(ProgramID, 1, Varyings, GL_INTERLEAVED_ATTRIBS);

	glLinkProgram(ProgramID); 
	glUseProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogSize);
	vector<char> ProgramErrorMessage( max(infoLogSize, int(1)) );
	glGetProgramInfoLog(ProgramID, infoLogSize, NULL, &ProgramErrorMessage[0]);
	
	if (&ProgramErrorMessage[0]) {
		fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vertShaderID);

	return ProgramID;
}