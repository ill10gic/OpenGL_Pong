#pragma once
#include <gl\glew.h>

class Mesh
{

public:
	// empty constructor
	Mesh();

	// create a mesh from the given vertices and vertex indices
	static void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	static void RenderMesh();
	static void ClearMesh();

	// destructor
	~Mesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
};

