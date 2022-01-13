#pragma once

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

//Function to read the shader text files
std::string get_file_contents(const char* filename);

//Class produces an OpenGL shader program that is nicely wrapped up
class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();

private:
	void compileErrors(unsigned int shader, const char* type);

};
