#include"shaderClass.h"

std::string get_file_contents(const char* filename) {
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		std::cout << contents << '\n';
		return contents;

	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	//Convert std strings to char arrays
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//Create a reference to an OpenGL shader. We must tell it the type of shader we want
	//vertexShader is a reference to the OpenGL vertex shader object that was created for us
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Give OpenGL the source code for the shader
	glShaderSource(vertexShader, 1, &vertexSource, NULL); //Second parm indicates 1 string for the source

	//The GPU cant understand the source code so we have to compile it into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX");

	//Do same for fragmentShader as for vertexShader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL); //Second parm indicates 1 string for the source
	glCompileShader(fragmentShader);
	compileErrors(vertexShader, "FRAGMENT");

	//In order to use these shaders, we have to wrap them into a shader program
	//Note there is only one type of shader program so no need for additional parameters
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	
	//Now we need to wrap up the shader program by linking the shaders together into a shader program
	glLinkProgram(ID);
	compileErrors(vertexShader, "PROGRAM");

	//We can now delete the shaders as they are now in the program itself and will not be needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	//Activate the shader program
	glUseProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type) {
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
}

void Shader::Delete() {
	glDeleteProgram(ID);
}