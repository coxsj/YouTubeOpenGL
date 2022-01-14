#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
	-0.5f,	0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	-0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.5f,  0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
	 0.5f,  0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
	 0.0f,  0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
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
	GLFWwindow* window = glfwCreateWindow(width, height, "YoutubeOpenGL", NULL, NULL);

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
	glViewport(0, 0, width, height);

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

	//Texture
	std::string pStr{ "resources/pots2k2k.png" };
	Texture pots(pStr.c_str(), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	pots.texUnit(shaderProgram, "tex0", 0);

	//Enables the depth buffer
	//Needed for discerning front vs back faces
	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	while (!glfwWindowShouldClose(window)) {
		//Draw a fresh background
		//Specify the background  color. This prepares open GL to do the clear on the back buffer.
		glClearColor(0.07f, 0.13f, 0.17, 1.0f);
		//Clear the back buffer assigning the new color to it
		//Also clear the GL depth buffer so a new depth set is calculated
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Draw our shapes
		//===============
		//Activate the shader program
		shaderProgram.Activate();

		camera.Inputs(window);
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Binds texture so that it appears in rendering
		pots.Bind();

		//Bind the VAO so OpenGL knows to use this one
		//Not strictly needed as we only have one object but it is good practice so OpenGL knows which vao to use
		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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