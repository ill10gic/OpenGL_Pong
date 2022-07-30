#pragma once
#include <vector>
#include <GL\glew.h>
#include <glm/glm/glm.hpp>

class Shape
{
public:

	void setIndices(std::vector<int>  inds)
	{
		indices = inds; 
	}

	void setVertices(std::vector<glm::vec3> verts)
	{
		vertices = verts;
	}

	std::vector<int> getIndices()
	{
		return indices;
	}

	std::vector<glm::vec3> getVertices()
	{
		return vertices;
	}

protected:
	std::vector<int> indices;
	int numIndices;
	int numVertices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> texCoords;
	std::vector<glm::vec3> normals;

};

