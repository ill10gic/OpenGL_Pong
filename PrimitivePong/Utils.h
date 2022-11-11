#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <gl/glew.h>
#include <glfw/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

using namespace std;

// Todo VAO,VBO, object methods - i may just replace all this
// Todo - better handlling for lack of forward declaration in header files 
struct VAO {
	GLuint val;
	GLuint posVBO;
	GLuint offsetVBO;
	GLuint sizeVBO;
	GLuint EBO;
};

class Utils
{
public:
	// MATH UTILS
	static float toRadians(float degrees);

	// TEXTURE UTILS
	static unsigned char* loadTexture(const char* texImagePath, int* width, int* height, bool useAlpha);
	static void freeTextureData(unsigned char* textureData);

	// SHADER UTILS
	static int GenShader(const char* filePath, GLenum type);
	static int GenShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
	static void BindShaderProgram(int shaderProgram);
	static void deleteShader(int shaderProgram);

	static std::string ReadFile(const char* fileLocation);

	// PROJECTION SETING UTILS
	static void setOrthographicProjection(int shaderId, float left, float right, float bottom, float top, float near, float far);

	

	/*
		Program Init & Window methods
	*/

	//	initialize glfw
	static void initGLFW(unsigned int versionMajor, unsigned int versionMinor);

	// create window; GLFWwindow*& is a pointer and then a reference to the pointer
	static void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferResizeCallback);

	// TODO load GLAD - will replace this with GLEW
	static bool loadGlad();

	// handle user input
	static void processInput(GLFWwindow* window, float* offset);

	// clear screen
	static void clearScreen();

	// new frame
	static void newFrame(GLFWwindow* window);

	// OBJECT VAO/VBO FUNCTIONS

	// generate VAO
	static void genVAO(VAO* vao);

	// generate buffer of certain type and set data
	template<typename T>
	static void genBufferObject(GLuint& bo, GLenum type, GLuint numOfElements, T* data, GLenum usage)
	{
		glGenBuffers(1, &bo); // create one buffer and assign id to IBO
		glBindBuffer(type, bo); // use this buffer we created
		glBufferData(type, numOfElements * sizeof(T), data, usage); // loads the buffer with our data
	}

	// update data in a buffer object
	template<typename T>
	static void updateData(GLuint& bo, GLintptr offset, GLuint numOfElements, T* data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bo); // set the buffer we want to modify
		// GL_ARRAY_BUFFER - targets vertex attribute
		// offset into the buffer's data where we will begin replacement
		// number of bytes we're replacing
		// the pointer to the new data that will be copied
		glBufferSubData(GL_ARRAY_BUFFER, offset, numOfElements * sizeof(T), data);  // update subset of data of buffer
	}

	// set attribute pointers
	template<typename T>
	static void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, bo); // set the buffer to be active
		glEnableVertexAttribArray(idx); // sets which shader "layout" variable we're modifying. E.G layout (location = 0) in vec3 pos;
		glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
		
		if (divisor > 0) {
			// reset _idx_ attribute every _divisor_ iteration through instances
			glVertexAttribDivisor(idx, divisor);
		}
	}

	// draw VAO
	static void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);

	// unbind buffer
	static void unbindBuffer(GLenum type);

	// unbind VAO
	static void unbindVAO();

	// cleans glfw
	static void cleanup();

	// deaollcate VAO/VBO memory
	static void cleanup(VAO vao);


	// ERROR CHECKING
	static bool checkOpenGLError();
	static void printShaderLog(GLuint shader);
	static void printProgramLog(GLuint shaderProgramId);
	static int checkShaderCompilationErrors(GLuint shader);
	static void CheckProgramLinkStatus(GLuint shaderProgramId);
};