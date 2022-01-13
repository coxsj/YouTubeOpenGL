#include "VBO.h"

VBO::VBO(GLfloat* vertices, GLsizeiptr size)
{
	//In order to transfer vertex info between the CPU and the GPU, we must create a vertex buffer object
	//(vbo is typically an array of references)
	glGenBuffers(1, &ID); //I beleive this function creates a general purpose OpenGL buffer
	//Make the VBO the current object (binded object)
	glBindBuffer(GL_ARRAY_BUFFER, ID);  //Must use GL_ARRAY_BUFFER type when referencing vertex buffer data
	//Transfer data to the buffer
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}