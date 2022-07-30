#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;
}

void Mesh::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	// create an Index Buffer Object
	glGenBuffers(1, &IBO); // create one index buffer object for this mesh and assign to Mesh's IBO private member
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // target this IBO/id for operations
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW); // load the data into the buffer

	glGenVertexArrays(1, &VAO); // create vertex array, passes ID into address of VAO
	glBindVertexArray(VAO); // target vertex array for operations

	glGenBuffers(1, &VBO); // create one buffer and assign to VBO, create a VBO for each VAO (there may be multiple VBOs per VAO)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // target VBO for operations

	glBufferData(GL_ARRAY_BUFFER, sizeof(indices[0]) * numOfVertices, vertices, GL_STATIC_DRAW); // load vertices into buffer

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0); // sets the VAO data to be used for vertex pos
	glEnableVertexAttribArray(0); // sets this to point to the "location 0" in the vertex data

	// for the textures, "location = 1" in vert shader, 2 componets (s,t) coords, stride of 8 to JUMP to next set of text coords, 
	// update the pointer/offset from the "beginning" of the generic vertex attribute AKA the whole "vertex"
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);

	// setup the data for the normal offset and enable it in the shader
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	// unbind so next operations can properly proceed
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::RenderMesh()
{
	glBindVertexArray(VAO); // bind the vertex array - work with this object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO); // bind index buffer object
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0); // render primitives from array data using the VAO and IBO given
	
	// unbind
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::ClearMesh()
{
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteBuffers(1, &VAO);
		VAO = 0;
	}
}

Mesh::~Mesh() 
{
	ClearMesh();
}
