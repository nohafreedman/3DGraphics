#include "Mesh.h"

Mesh::~Mesh()
{
	// clears vertex array and buffers
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
}

void Mesh::initialiseQuad()
{
	// asserts that the vertex array has not been made yet
	assert(vao == 0);

	// generates buffers
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// binds vertex array to our variable
	glBindVertexArray(vao);
	// bind vertex buffer to our variable
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	Vertex vertices[6];
	vertices[0].position = { -0.5f, 0, 0.5f, 1 };
	vertices[1].position = { 0.5f, 0, 0.5f, 1 };
	vertices[2].position = { -0.5f, 0, -0.5f, 1 };

	vertices[3].position = { -0.5f, 0, -0.5f, 1 };
	vertices[4].position = { 0.5f, 0, 0.5f, 1 };
	vertices[5].position = { 0.5f, 0, -0.5f, 1 };

	// fill vertex buffer. creates buffer the size of 6 Vertex objects, fills buffer with vertices
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	triCount = 2;

}

void Mesh::initialise(unsigned int vertexCount,
	const Vertex* vertices,
	unsigned int indexCount,
	unsigned int* indices) {
	assert(vao == 0);

	// generate buffers for GPU
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	// bind vertex array, essentially stores our vertexes and attributes
	glBindVertexArray(vao);

	// bind vertex buffer, which gets its info written to the vao
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// fill the buffers with the data inside vertices
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);

	// begin binding vertex attributes
	glEnableVertexAttribArray(0);
	// 6 parameters:
	//	- which attribute to define
	//	- how many elements the attribute has
	//	- what type of data the attribute is
	//	- should it normalize the attribute (for vectors)
	//	- how many bytes seperate each attribute
	//	- how many bytes from the start of the vertex data until the first attribute
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	// normal attributes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)16);

	if (indexCount != 0) {
		glGenBuffers(1, &ibo);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

		triCount = indexCount / 3;
	}
	else {
		triCount = vertexCount / 3;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Mesh::draw()
{
	glBindVertexArray(vao);

	if (ibo != 0)
		glDrawElements(GL_TRIANGLES, 3 * triCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 *  triCount);

}
