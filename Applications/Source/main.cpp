#define GLEW_STATIC
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <SOIL.h>
#include <cmath>
#include <stdio.h>

#include <Shader.h>

GLFWwindow* window = nullptr;

const std::string pathToShaders("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Resources\\Shaders\\");
const std::string pathToTextures("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Resources\\Textures\\");


// VAO, VBO, EBO, TBO
GLuint VAO{ 0 };
GLuint VBO{ 0 };
GLuint EBO{ 0 };
GLuint TBO[2] { 0, 0 };

// Shader program
Shader* shader = nullptr;

// Textures
struct Texture {
	unsigned char* image;
	int width, height;
} texture, lena;


// auxillary flags
const bool isIndexedDraw = false;
const bool isDebugEnabled = true;

GLfloat vertices[] = {
	// Positions         // Colors         // Tex Cords 
	0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f, 0.0f, // Bottom Right
	-0.5f, -0.5f, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
	0.0f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f, 0.5f, 1.0f  // Top 
};

GLuint indices[] = { 0, 1, 2 };

static void loadTexture(void)
{
	std::string pathToTexture = pathToTextures + "texture.jpg";
	texture.image = SOIL_load_image(pathToTexture.c_str(), &texture.width, &texture.height, 0, SOIL_LOAD_RGB);

	std::string pathToLena = pathToTextures + "lena.png";
	lena.image = SOIL_load_image(pathToLena.c_str(), &lena.width, &lena.height, 0, SOIL_LOAD_RGB);

	glGenTextures(2, TBO);

	// texture.jpeg
	glBindTexture(GL_TEXTURE_2D, TBO[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.image);
	glGenerateMipmap(GL_TEXTURE_2D);

	// lena.png
	glBindTexture(GL_TEXTURE_2D, TBO[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, lena.width, lena.height, 0, GL_RGB, GL_UNSIGNED_BYTE, lena.image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(texture.image);
	SOIL_free_image_data(lena.image);
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

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, 640, 480);

	shader = new Shader(pathToShaders + "vertex.vert", pathToShaders + "fragment.frag");

	loadTexture();

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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*) 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 6));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void uninitialize(void) 
{
	glDeleteBuffers(2, TBO);
	if (isIndexedDraw) {
		glDeleteBuffers(1, &EBO);
	}
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
}

void render(void)
{
	GLint transformLocation { -1 };

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader->UseProgram();
		glBindVertexArray(VAO);

		// textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TBO[0]);
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "wallTexture"), 1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TBO[1]);
		glUniform1i(glGetUniformLocation(shader->GetProgram(), "lenaTexture"), 0);

		// transformations
		glm::mat4 transform;
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, static_cast<GLfloat>(glfwGetTime()), glm::vec3(0.0f, 0.0f, 1.0f));
		transformLocation = glGetUniformLocation(shader->GetProgram(), "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		if (isIndexedDraw) {
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		} else {
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

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
