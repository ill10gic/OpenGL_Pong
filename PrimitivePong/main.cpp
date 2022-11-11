#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Utils.h"

// Global Settings

// opengl settings
unsigned int versionMajor = 4;
unsigned int versionMinor = 3;

// window settings
unsigned int scrWidth = 800;
unsigned int scrHeight = 600;
const char* title = "PrimitivePong";

GLuint shaderProgram;


// callback for windows size change
void framebufferResizeCallback(GLFWwindow* window, int width, int height) // pass the scrWidth and Height by reference
{
	glViewport(0, 0, width, height);
	scrWidth = width;
	scrHeight = height;

	// update projection matrix
	Utils::setOrthographicProjection(shaderProgram, 0, width, 0, height, 0.0f, 1.0f);
}

int main() {
	std::cout << "Hello, N64" << std::endl;

	// handle delta timing
	double dt = 0.0f;
	double lastFrame = 0.0f;

	// initialization
	Utils::initGLFW(versionMajor, versionMinor);

	// create window
	GLFWwindow* window = nullptr;

	Utils::createWindow(window, title, scrWidth, scrHeight, framebufferResizeCallback);

	if (!window) {
		std::cout << "Could not create window" << std::endl;
		Utils::cleanup();
		return -1;
	}

	if (glewInit() != GLEW_OK) { // GLEW uses its own enums here!!!
		printf("GLEW initialization failed");
		glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}

	glViewport(0, 0, scrWidth, scrHeight);
	// Shader setup
	shaderProgram = Utils::GenShaderProgram("main.vs", "main.fs");
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
	Utils::genBufferObject<float>(vao.posVBO, GL_ARRAY_BUFFER, 2 * 4, vertices, GL_STATIC_DRAW);
	Utils::setAttPointer<float>(vao.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	Utils::genBufferObject<float>(vao.offsetVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW);
	Utils::setAttPointer<float>(vao.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	// size VBO
	Utils::genBufferObject<float>(vao.sizeVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW);
	Utils::setAttPointer<float>(vao.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 1);

	// EBO
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
		Utils::processInput(window, offsets);

		// update data
		Utils::updateData<float>(vao.offsetVBO, 0, 1 * 2, offsets);

		//  clear screen for new frame
		Utils::clearScreen();

		// render object
		Utils::BindShaderProgram(shaderProgram);
		Utils::draw(vao, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);
		Utils::checkOpenGLError();

		// swap frames
		Utils::newFrame(window);
	}

	// cleanup memory
	Utils::cleanup(vao);
	Utils::deleteShader(shaderProgram);
	Utils::cleanup();
	return 0;

}