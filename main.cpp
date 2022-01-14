#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // Upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f  // Lower right corner
};

GLuint indices[] =
{
	0, 2, 1, // Upper triangle
	0, 3, 2 // Lower triangle
};


int main() 
{
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	//In this case we are using OpenGL 3.3
	//Tell GLFW we are using the core profile
	//i.e. we are only using the modern functions
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create a GLFWwindow object of 800 x 800 pixels naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);

	//Handle case where window does not get created
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window/n";
		glfwTerminate();
		return -1;
	}

	//Introduce the window into the current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	//Specify the viewport of OpenGL in the window
	// Viewport goes from 0,0 (lower left) to 800, 800 (upper right)
	glViewport(0, 0, 800, 800);

	//Create shaders and buffers
	//==========================
	//Create Shader object using default shaders
	Shader shaderProgram("default.vert", "default.frag");

	//Generate Vertex Array object and bind it
	VAO VAO1;
	VAO1.Bind();

	//Generate Vertex Buffer object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	//Generate Element Buffer and link it to indices
	EBO EBO1(indices, sizeof(indices));

	//Link VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	//unbind all to prevent accidentally modifying any of them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Get a reference to the uniform variable "scale" in the shader
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture
	std::string pStr{ "resources/pots2k2k.png" };
	Texture pots(pStr.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pots.texUnit(shaderProgram, "tex0", 0);

	while (!glfwWindowShouldClose(window)) {
		//Draw a fresh background
		//Specify the background  color. This prepares open GL to do the clear on the back buffer.
		glClearColor(0.07f, 0.13f, 0.17, 1.0f);
		//Clean the back buffer and assigne the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our shapes
		//Activate the shader program
		shaderProgram.Activate();
		// Assigns a value to the uniform; NOTE: Must always be done after activating the Shader Program
		glUniform1f(uniID, 0.5f);	//Note there are different suffix endings to this function for different numbers of and types of variable
		// Binds texture so that is appears in rendering
		pots.Bind();

		//Bind the VAO so OpenGL knows to use this one
		//Not strictly needed as we only have one object but it is good practice so OpenGL knows which vao to use
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//Now that we've drawn the shapes, swap the buffers
		glfwSwapBuffers(window);

		//Take care of all GLFW events
		glfwPollEvents();
	};

	//Delete objects created in this routine
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	pots.Delete();
	shaderProgram.Delete();

	//Destroy the window before ending the program
	glfwDestroyWindow(window);

	//Clear out GLFW before ending the program
	glfwTerminate();
	return 0;
}