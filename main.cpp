#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (Location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


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

	GLfloat vertices[] =
	{
		-0.5f,	-0.5f * float(sqrt(3)) / 3, 0.0f,
		0.5f,	-0.5f * float(sqrt(3)) / 3, 0.0f,
		0.0f,	0.5f * float(sqrt(3)) * 2 / 3,	0.0f

	};

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

	//Create a reference to an OpenGL shader. We must tell it the type of shader we want
	//vertexShader is a reference to the OpenGL vertex shader object that was created for us
	GLuint vertexShader = glad_glCreateShader(GL_VERTEX_SHADER); 
	//Give OpenGL the source code for the shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //Second parm indicates 1 string for the source

	//The GPU cant understand the source code so we have to compile it into machine code
	glCompileShader(vertexShader);

	//Do same for fragmentShader as for vertexShader
	GLuint fragmentShader = glad_glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //Second parm indicates 1 string for the source
	glCompileShader(fragmentShader);

	//In order to use these shaders, we have to wrap them into a shader program
	//Note there is only one type of shader program so no need for additional parameters
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Now we need to wrap up the shader program by linking the shaders together into a shader program
	glLinkProgram(shaderProgram);

	//We can now delete the shaders as they are now in the program itself and will not be needed
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//In order to transfer vertex info between the CPU and the GPU, we must create a vertext buffer object
	//(vbo is typically an array of references)
	//Also must create a vertex array object. VAO is used by OpenGL to quickly switch between VBOs.
	//VAO must be generated BEFORE the VBO
	GLuint VAO, VBO;		//Reference to the vertex array object and the vertex buffer object


	//Generate VAO and VBO with only one object in each
	glGenVertexArrays(1, &VAO); //Must be generated before the VBO
	glGenBuffers(1, &VBO); //I beleive this function creates a general purpose buffer

	//Bind the VAO so we can work with is
	glBindVertexArray(VAO);

	//Make the VBO the current object (binded object)
	glBindBuffer(GL_ARRAY_BUFFER, VBO);  //Must use GL_ARRAY_BUFFER type when referencing vertex buffer data

	//Transfer data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Configure VAO so OpenGL knows how to read the VBO
	//We do this using attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//Enable the attribute pointer we just created so OpenGL knows to use it
	glEnableVertexAttribArray(0); //Note that 0 is the position of the vertex attribute. See first parameter in glVertexAttribPointer

	//Not mandatory, nice to have
	//Can now be 100% sure you wont accidentally modify a vbo with an OpenGL function.
	glBindBuffer(GL_ARRAY_BUFFER, 0);  //i.e. binds no buffer
	//Do same for VAO
	//Order is very important here. Note the "debinding" of VAO occurs AFTER the debinding of vbo
	//This is the opposite of the order that matters when generating these objects
	glBindVertexArray(0);  //i.e. binds no buffer


	//Specify the background  color. This prepares open GL to do the clear on the back buffer.
	glClearColor(0.07f, 0.13f, 0.17, 1.0f); 

	//Clean the back buffer and assigne the new color to it
	glClear(GL_COLOR_BUFFER_BIT);

	//Swap the back buffer with the front buffer
	glfwSwapBuffers(window);


	while (!glfwWindowShouldClose(window)) {
		//Draw a fresh background
		//Specify the background  color. This prepares open GL to do the clear on the back buffer.
		glClearColor(0.07f, 0.13f, 0.17, 1.0f);
		//Clean the back buffer and assigne the new color to it
		glClear(GL_COLOR_BUFFER_BIT);

		//Draw our shapes
		//Activate the shader program
		glUseProgram(shaderProgram);
		//Bind the VAO so OpenGL knows to use this one
		//Not strictly needed as we only have one object but it is good practice so OpenGL knows which vao to use
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//Now that we've drawn the shapes, swap the buffers
		glfwSwapBuffers(window);

		//Take care of all GLFW events
		glfwPollEvents();
	};
	glfwDestroyWindow(window);

	//Delete objects created in this routine
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}