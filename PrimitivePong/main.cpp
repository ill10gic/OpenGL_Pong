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

// 2d vector struct
struct vec2 {
	float x;
	float y;
};

// ball variables
const float ballDiameter = 16.0f;
const float ballRadius = 16.0f / 2.0f;

/*	Vector velocity logic
	Ball's vector will be moved by <x, y> = <x, y> + velocityVector * deltaTime
	Collision are Elastic collisions (no loss of energy) velocity does not decrease
	Velocity can increase. Faster moving paddle can increase velocity

	if paddle is moving up and ball hit paddle moving up, negate the x component of ball vector
	but will modify the y component of ball vector to speed or slow ball
*/

/*	collision detection logic
	
	For top of bounding box:
	if ball radius, plus the y component of ball's offset is greater than scrn height, a collision occurred

	for right side of bounding box:
	if ball radius, plus x component of ball's offset is greater than scrn width, a collision happened

	for bottom:
	if ball's y minus radius is less than zero, a collision happened


*/

// paddle variables
const float paddleHeight = 100.0f;
const float paddleWidth = 10.0f;
const float halfPaddleHeight = paddleHeight / 2.0f;
const float halfPaddleWidth = paddleWidth / 2.0f;
const float offset = ballRadius;
const float paddleBoundary = halfPaddleHeight + offset;
const float paddleMovementSpeed = 75.0f;

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

	/*
		Paddle VAO/VBOs
	*/

	// paddle vertex data
	float paddleVertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	// paddle vbo indice data - counter clockwise winding order
	unsigned int paddleIndices[] = {
		0, 1, 2, // top left triangle
		2, 3, 0 // bottom right triangle
	};

	// paddle offsets
	float paddleOffsets[] = {
		35.0f, scrHeight / 2.0f, // left paddle
		scrWidth - 35.0f, scrHeight / 2.0f // righht paddle
	};

	// size array
	float paddleSizes[] = {
		paddleWidth, paddleHeight // only need to specify once
	};

	// setup VAO
	VAO paddleVAO;
	Utils::genVAO(&paddleVAO);

	// pos VBO
	Utils::genBufferObject<float>(paddleVAO.posVBO, GL_ARRAY_BUFFER, 2 * 4, paddleVertices, GL_STATIC_DRAW);
	Utils::setAttPointer<float>(paddleVAO.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	Utils::genBufferObject(paddleVAO.offsetVBO, GL_ARRAY_BUFFER, 2 * 2, paddleOffsets, GL_DYNAMIC_DRAW);
	Utils::setAttPointer<float>(paddleVAO.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	// size VBO
	Utils::genBufferObject(paddleVAO.sizeVBO, GL_ARRAY_BUFFER, 2 * 1, paddleSizes, GL_STATIC_DRAW);
	Utils::setAttPointer<float>(paddleVAO.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 2); // dont set divisor because this doesn't change between instances


	// paddle EBO
	Utils::genBufferObject<GLuint>(paddleVAO.EBO, GL_ELEMENT_ARRAY_BUFFER, 2 * 3, paddleIndices, GL_STATIC_DRAW);

	// unbind
	Utils::unbindBuffer(GL_ARRAY_BUFFER);
	Utils::unbindVAO();

	/*
		Ball VAO
	*/

	float* ballVertices;
	unsigned int* ballIndices;
	unsigned int numOfTriangles = 50;

	Utils::Gen2DCircleArray(ballVertices, ballIndices, numOfTriangles, 0.5f);

	// offsets array - what is this for?
	float ballOffsets[] = {
		scrWidth / 2, scrHeight / 2
	};

	// size array
	float ballSizes[] = {
		ballDiameter, ballDiameter
	};

	// setup VAO/VBOs
	VAO ballVao;
	Utils::genVAO(&ballVao);

	// pos VBO
	Utils::genBufferObject<float>(ballVao.posVBO, GL_ARRAY_BUFFER, (numOfTriangles + 1) * 2, ballVertices, GL_STATIC_DRAW);
	Utils::setAttPointer<float>(ballVao.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	Utils::genBufferObject<float>(ballVao.offsetVBO, GL_ARRAY_BUFFER, 1 * 2, ballOffsets, GL_DYNAMIC_DRAW);
	Utils::setAttPointer<float>(ballVao.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	// size VBO
	Utils::genBufferObject<float>(ballVao.sizeVBO, GL_ARRAY_BUFFER, 1 * 2, ballSizes, GL_STATIC_DRAW);
	Utils::setAttPointer<float>(ballVao.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 1);

	// EBO
	Utils::genBufferObject<unsigned int>(ballVao.EBO, GL_ELEMENT_ARRAY_BUFFER, 3 * numOfTriangles, ballIndices, GL_STATIC_DRAW);

	// unbind VBO and VAO
	Utils::unbindBuffer(GL_ARRAY_BUFFER);
	Utils::unbindVAO();

	// render loop ...
	while (!glfwWindowShouldClose(window)) {
		// update time
		dt = glfwGetTime() - lastFrame;
		lastFrame += dt;

		// input
		Utils::processInput(window, paddleOffsets, dt, scrHeight, paddleBoundary);

		// update data
		Utils::updateData<float>(paddleVAO.offsetVBO, 0, 2 * 2, paddleOffsets);
		Utils::updateData<float>(ballVao.offsetVBO, 0, 1 * 2, ballOffsets);

		//  clear screen for new frame
		Utils::clearScreen();

		// render object
		Utils::BindShaderProgram(shaderProgram);
		Utils::draw(ballVao, GL_TRIANGLES, 3 * numOfTriangles, GL_UNSIGNED_INT, 0);
		Utils::draw(paddleVAO, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0, 2);
		Utils::checkOpenGLError();

		// swap frames
		Utils::newFrame(window);
	}

	// cleanup memory
	Utils::cleanup(ballVao);
	Utils::cleanup(paddleVAO);
	Utils::deleteShader(shaderProgram);
	Utils::cleanup();
	return 0;

}