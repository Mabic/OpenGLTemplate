#define GLEW_STATIC
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <cmath>
#include <stdio.h>

#include "Shader.h"

GLFWwindow* window = nullptr;

// VAO, VBO, EBO
GLuint VAO;
GLuint VBO;
GLuint EBO;

Shader* shader;

const bool isIndexedDraw = false;
const bool isDebugEnabled = true;

GLfloat vertices[] = {
	// Positions         // Colors
	0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // Bottom Left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f // Top 
};

GLuint indices[] = { 0, 1, 2 };

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

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	shader = new Shader("vertex.vert", "fragment.frag");

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	if (isIndexedDraw) {
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 6, (GLvoid*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
}

void uninitialize(void) 
{
	if (isIndexedDraw) {
		glDeleteBuffers(1, &EBO);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
}

void render(void)
{
	GLfloat offset[] { 0.0f, 0.0f, 0.0f };
	GLint offsetLocation { -1 };

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->UseProgram();
		glBindVertexArray(VAO);

		// change offset variable in shaders	
		offset[0] = (sin(glfwGetTime()) / 3);
		offset[1] = (sin(glfwGetTime()) / 4);
		offset[2] = (sin(glfwGetTime()) / 5);
		offsetLocation = glGetUniformLocation(shader->GetProgram(), "offset");
		glUniform3fv(offsetLocation, 1, offset);

		if (isIndexedDraw) {
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		} else {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

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