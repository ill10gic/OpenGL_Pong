#include <glfw/glfw3.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "Utils.h"

// settings
unsigned int scrWidth = 800;
unsigned int scrHeight = 600;
const char* title = "PrimitivePong";

int main() {
	std::cout << "Hello, N64" << std::endl;

	// handle delta timing
	double dt = 0.0f;
	double lastFrame = 0.0f;

	// initialization
	Utils::initGLFW(4, 6);

	// create window
	GLFWwindow* window = nullptr;

	Utils::createWindow(window, title, scrWidth, scrHeight, Utils::framebufferSizeCallback);

	if (!window) {
		std::cout << "Could not create window" << std::endl;
		Utils::cleanup();
		return -1;
	}

	// TODO GLAD stuff... replace with GLEW

	// Shader setup
	GLuint shaderProgram = Utils::GenShaderProgram("main.vs", "main.fs");
	Utils::setOrthographicProjection(shaderProgram, 0, scrWidth, 0, scrHeight, 0.0f, 1.0f);

	// setup vertex data - general template for a square - will scale it to whatever we want to
	float vertices[] = {
		//	x		y
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	// index array - because vertices 0, 2 will overlap
	unsigned int indices[] = {
		0, 1, 2, // triangle 1 - top left
		2, 3, 0  // triangle 2 - bottom right
	};

	// offsets array - what is this for?
	float offsets[] = {
		200.0f, 200.0f
	};

	// size array
	float sizes[] = {
		50.0f, 50.0f
	};

	// setup VAO/VBOs
	VAO vao;
	Utils::genVAO(&vao);

	// pos VBO
	Utils::genBufferObject<float>(vao.posVBO, GL_ARRAY_BUFFER, 2 * 4, vertices, GL_STATIC_DRAW); // static draw - data doesn't change
	Utils::setAttPointer<float>(vao.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	Utils::genBufferObject<float>(vao.offsetVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW); // dynamic draw because we may want to use this and change it
	Utils::setAttPointer<float>(vao.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	// size VBO
	Utils::genBufferObject<float>(vao.sizeVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW);
	Utils::setAttPointer<float>(vao.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 1);

	// EBO - element buffer object/ sometimes referred to as index buffer object or IBO
	Utils::genBufferObject<unsigned int>(vao.EBO, GL_ELEMENT_ARRAY_BUFFER, 3 * 2, indices, GL_STATIC_DRAW);

	// unbind VBO and VAO
	Utils::unbindBuffer(GL_ARRAY_BUFFER);
	Utils::unbindVAO();

	// render loop ...
	while (!glfwWindowShouldClose(window)) {
		// update time
		dt = glfwGetTime() - lastFrame;
		lastFrame += dt;

		// input
		Utils::processInput(window);
	}
}