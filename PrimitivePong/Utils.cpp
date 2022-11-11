#include "Utils.h"

float Utils::toRadians(float degrees)
{
    return degrees * 2.0f * 3.14159f / 360.0f;
}

void Utils::initGLFW(unsigned int versionMajor, unsigned int versionMinor)
{
	// Initialize GLFW
	if (!glfwInit()) { // Todo - Setup a "GLFW Teardown" util function if it doesn't exist
		printf("GLFW Initialization failed!");
		glfwTerminate();
	}

	glfwSwapInterval(1); // Enables Vertical sync

	// Setup GLFW window properties
	// OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);

	// make it not backwards compatible or using old OpenGL3.3 type code..
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // MACOS specific setup ??
}

void Utils::createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferResizeCallback)
{
	window = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!window) {
		Utils::cleanup();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void Utils::processInput(GLFWwindow* window, float* offset)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		offset[1] += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		offset[0] += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		offset[1] -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		offset[0] -= 1.0f;
	}
}

void Utils::clearScreen()
{
	glClearColor(0.0, 0.0, 0.0, 1.0); // set color to clear to
	glClear(GL_COLOR_BUFFER_BIT); // actually clears buffer
}

void Utils::newFrame(GLFWwindow* window)
{
	// basic rendering pipeline output is to display one frame while processing the next frame to display
	glfwSwapBuffers(window); // swap for the rendered frame
	glfwPollEvents(); // check for events from the keyboard
}

void Utils::cleanup()
{
	glfwTerminate();
}

int Utils::GenShader(const char* filePath, GLenum type)
{
	std::string shaderSrc = Utils::ReadFile(filePath); // get the source code of the shader
	const char* shaderStr = shaderSrc.c_str();

	// build and compile shader
	int shaderId = glCreateShader(type); // creates an (initially) empty shader object on GPU - returns an ID for it
	glShaderSource(shaderId, 1, &shaderStr, NULL); // the shader object (id) to store the shader program, the number of strings in the shader source code, array of pointers to the strings of source code, NULL terminated string if NULL, otherwise an array of lengtsh for each string
	glCompileShader(shaderId); // compile

	// check for errors
	return Utils::checkShaderCompilationErrors(shaderId) == -1 ? -1 : shaderId;
}

int Utils::GenShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath)
{
	int shaderProgram = glCreateProgram();

	int vertexShader = GenShader(vertexShaderPath, GL_VERTEX_SHADER);
	int fragmentShader = GenShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	if (vertexShader == -1 || fragmentShader == -1) {
		return -1;
	}

	// link shaders
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// check errors
	CheckProgramLinkStatus(shaderProgram);

	// can delete the shaders in this instance since they are already linked & compiled
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void Utils::BindShaderProgram(int shaderId)
{
	glUseProgram(shaderId); // set opengl to use this program
}

void Utils::deleteShader(int shaderProgram)
{
	if (shaderProgram != 0) {
		glDeleteProgram(shaderProgram);
	}
}

std::string Utils::ReadFile(const char* fileName)
{
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";
	// open file
	file.open(fileName);
	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open " << fileName << std::endl;
	}
	file.close();
	return ret;
}

void Utils::setOrthographicProjection(int shaderId, float left, float right, float bottom, float top, float near, float far)
{
	glm::mat4 orthoMat = glm::ortho(left, right, bottom, top, near, far);

	BindShaderProgram(shaderId);
	glUniformMatrix4fv(glGetUniformLocation(shaderId, "projection"), 1, GL_FALSE, &orthoMat[0][0]);
}

/// <summary>
/// Render mesh
/// </summary>
/// <param name="vao"></param>
/// <param name="mode"></param>
/// <param name="count"></param>
/// <param name="type"></param>
/// <param name="indices"></param>
/// <param name="instanceCount"></param>
void Utils::draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount)
{
	glBindVertexArray(vao.val); // use the ID of our VAO to make active.

	// glDrawElementsInstanced allows us to use EBO - "Elements"?
	glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);

	// glDrawArrays(mode, 0, count);
}

void Utils::unbindBuffer(GLenum type)
{
	glBindBuffer(type, 0);
}

void Utils::unbindVAO()
{
	glBindVertexArray(0); // clear up bound object giving a little bit more space
}

void Utils::cleanup(VAO vao)
{
	glDeleteBuffers(1, &vao.posVBO);
	glDeleteBuffers(1, &vao.offsetVBO);
	glDeleteBuffers(1, &vao.sizeVBO);
	glDeleteBuffers(1, &vao.EBO);
	glDeleteVertexArrays(1, &vao.val);
}

bool Utils::checkOpenGLError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}

void Utils::printShaderLog(GLuint shader) {
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
}

void Utils::printProgramLog(GLuint shaderProgramId)
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetProgramiv(shaderProgramId, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(shaderProgramId, len, &chWritten, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
}

int Utils::checkShaderCompilationErrors(GLuint shader)
{
	int len = 0;
	int chWritten = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWritten, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
		return -1;
	}
	return 0;
}

void Utils::CheckProgramLinkStatus(GLuint shaderProgramId)
{
	GLint linked;
	glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &linked);
	if (!linked) {
		cout << "linking failed" << endl;
		Utils::printProgramLog(shaderProgramId);
	}
}

void Utils::genVAO(VAO* vao)
{
	glGenVertexArrays(1, &vao->val); // creates memory space for the vertex array - passes new ID into VAO
	glBindVertexArray(vao->val); // any open GL functions we used will work on this VAO now
}
