#pragma once
#include <gl/glew.h>
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
	static int BindShaderProgram(int shaderProgram);
	static void deleteShader(int shaderProgram);

	// PROJECTION SETING UTILS
	static void setOrthographicProjection(int shaderProgram, float left, float right, float bottom, float top, float near, float far);

	

	/*
		Program Init & Window methods
	*/

	//	initialize glfw
	static void initGLFW(unsigned int versionMajor, unsigned int versionMinor);

	// create window; GLFWwindow*& is a pointer and then a reference to the pointer
	static void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback);

	// callback for windows size change
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	// TODO load GLAD - will replace this with GLEW
	static bool loadGlad();

	// handle user input
	static void processInput(GLFWwindow* window);

	// clear screen
	void clearScreen();

	// new frame
	void newFrame(GLFWwindow* window);

	// cleanup
	static void cleanup();

	// OBJECT VAO/VBO FUNCTIONS

	// generate VAO
	static void genVAO(VAO* vao);

	// generate buffer of certain type and set data
	template<typename T>
	static void genBufferObject(GLuint& bo, GLenum type, GLuint numOfElements, T* data, GLenum usage);

	// update data in a buffer object
	template<typename T>
	void updateData(GLuint& bo, GLintptr offset, GLuint numOfElements, T* data);

	// set attribute pointers
	template<typename T>
	static void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0);

	// draw VAO
	void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);

	// unbind buffer
	static void unbindBuffer(GLenum type);

	// unbind VAO
	static void unbindVAO();

	// deaollcate VAO/VBO memory
	void cleanup(VAO vao);


};

// Todo VAO,VBO, object methods - i may just replace all this
struct VAO {
	GLuint val;
	GLuint posVBO;
	GLuint offsetVBO;
	GLuint sizeVBO;
	GLuint EBO;
};