#define GLEW_STATIC
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <memory>
#include <stdio.h>
#include <SOIL.h>

#include "Camera.hpp"
#include "Shader.h"

const std::string pathToShaders("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Scene\\Resources\\Shaders\\");
const std::string pathToTexture("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Scene\\Resources\\Textures\\");


// VAO, VBO, EBO
GLuint VAO{ 0 };
GLuint VBO{ 0 };
GLuint EBO{ 0 };
GLuint TBO{ 0 };

// Textures
struct Texture {
	unsigned char* image;
	int width, height;
} texture;

std::unique_ptr<Shader> shader;

GLFWwindow* window = nullptr;

glm::vec3 positions[]{
	{0.0f, -2.7f, -3.0f}, // 1
	{0.0f, -0.5f, -4.0f}  // 2
};

GLfloat cube[] {         // TexCoord
	 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 0 
	-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 1
	-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, // 2
	 0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 3
	 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
	-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 5
	-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 6
	 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 7
};

GLuint cubeIndices[]{
	// front
	0, 1, 2,
	0, 2, 3,
	// left
	1, 5, 6,
	1, 6, 2,
	// right
	0, 3, 7,
	0, 7, 4,
	// back
	4, 6, 5,
	4, 7, 6,
	// up
	3, 2, 6,
	3, 6, 7,
	// down
	1, 0, 5,
	0, 5, 4,
};

// Camera variables
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
bool firstMouse = true;

void mouse_callback(GLFWwindow* window, double positionX, double positionY)
{
	if (firstMouse)
	{
		lastX = static_cast<GLfloat>(positionX);
		lastY = static_cast<GLfloat>(positionY);
		firstMouse = false;
	}
	GLfloat xoffset = static_cast<GLfloat>(positionX) - lastX;
	GLfloat yoffset = lastY - static_cast<GLfloat>(positionY);

	lastX = static_cast<GLfloat>(positionX);
	lastY = static_cast<GLfloat>(positionY);

	camera.UpdateEulerAngles(xoffset, yoffset);
}

static void init(void)
{
	// start GL context and O/S window using the GLFW helper library
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return;
	}

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(640, 480, "Hello Triangle", NULL, NULL);

	if (!window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 640, 480);
	glEnable(GL_DEPTH_TEST);

	shader.reset(new Shader(pathToShaders + "vertex.vert", pathToShaders + "fragment.frag"));

	// Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Vertex Buffer Object for Cube
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), &cube, GL_STATIC_DRAW);

	// Element Buffer Object for Cube
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), &cubeIndices, GL_STATIC_DRAW);

	// Set current VAO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*) 0);
	glEnableVertexAttribArray(0);

	// Load Texture
	std::string path(pathToTexture + "texture.jpg");
	texture.image = SOIL_load_image(path.c_str(), &texture.width, &texture.height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &TBO);
	glBindTexture(GL_TEXTURE_2D, TBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.image);
	glGenerateMipmap(GL_TEXTURE_2D);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (GLvoid*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);

	SOIL_free_image_data(texture.image);

	// Unbind current VAO
	glBindVertexArray(0);

	// Unbind current texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void uninitialize(void) 
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
}

void render(void)
{
	GLfloat angle = 0.0f;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->UseProgram();
		glBindVertexArray(VAO);
		
		// textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TBO);
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureSampler"), 0);

		
		// transformations
		glm::mat4 modelMatrix;
		angle -= 0.05f;
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.7f, -3.0f));
		modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));

		glm::mat4 viewMatrix = camera.GetViewMatrix();

		glm::mat4 projectionMatrix = glm::perspective(45.0f, static_cast<float>(640 / 480), 0.1f, 100.0f);
		
		// pass transformation to vertex shader
		GLint modelLocation = glGetUniformLocation(shader->GetProgram(), "model");
		GLint viewLocation = glGetUniformLocation(shader->GetProgram(), "view");
		GLint projectionLocation = glGetUniformLocation(shader->GetProgram(), "projection");

		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.0f, -2.0f));
		
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

int main() 
{
	init();
	render();
	uninitialize();
	return 0;
}