#pragma once
#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>

#include "shaderClass.h"

//Function to read the shader text files
std::string get_file_contents(const char* filename);

//Class produces an OpenGL texture class
class Texture
{
public:
	GLuint ID;
	GLenum type;
	Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

	//Assigns a texture unit to a texture
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	//binds a texture
	void Bind();
	//Unbinds a texture
	void Unbind();
	//Deletes a texture
	void Delete();

private:
	void compileErrors(unsigned int shader, const char* type);

};
