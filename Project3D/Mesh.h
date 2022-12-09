#pragma once
#include <gl_core_4_4.h>
#include <glm/glm.hpp>
using namespace glm;

class Mesh
{
public:

	Mesh() : triCount(0), vao(0), vbo(0), ibo(0) {}
	virtual ~Mesh();

	struct Vertex {
		vec4 position;
		vec4 normal;
		vec2 texCoord;
	};

	void initialiseQuad();
	void initialise(unsigned int vertexCount,
					const Vertex* vertices,
					unsigned int indexCount = 0,
					unsigned int* indicies = nullptr);
	virtual void draw();

protected:

	unsigned int triCount;
	// vao is a wrapper for the mesh, combines the vertex buffer vbo with the index buffer ibo
	// vertex buffer vbo stores vertexes
	// index buffer ibo stores indexes
	// VERTEX ARRAY OBJECT: VAO
	// VERTEX BUFFER OBJECTS: VBO
	// INDEX BUFFER OBJECT: IBO
	unsigned int vao, vbo, ibo;
};

