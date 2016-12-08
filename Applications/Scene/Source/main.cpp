#define GLEW_STATIC
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>    // include GLEW and new version of GL on Windows
#include <GLFW/glfw3.h> // GLFW helper library
#include <memory>
#include <stdio.h>

#include "Mesh.hpp"
#include "Camera.hpp"
#include "Texture.hpp"
#include "Shader.h"
#include "ModelLoader.hpp"

const std::string pathToShaders("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Scene\\Resources\\Shaders\\");
const std::string pathToTexture("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Scene\\Resources\\Textures\\");
const std::string pathToModel("C:\\Users\\mariu\\OneDrive\\Dokumenty\\Visual Studio 2015\\Projects\\OpenGLTemplate\\Applications\\Scene\\Resources\\Models\\");

std::vector<unsigned int> indicesSize;

// VAO, VBO, EBO	   
std::vector<GLuint> VAO;
std::vector<GLuint> VBO;
std::vector<GLuint> EBO;
std::vector<GLuint> TBO;

std::unique_ptr<Shader> shader;

GLFWwindow* window = nullptr;

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

    window = glfwCreateWindow(1024, 768, "Hello Triangle", NULL, NULL);

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

    glViewport(0, 0, 1024, 768);
    glEnable(GL_DEPTH_TEST);

    shader.reset(new Shader(pathToShaders + "vertex.vert", pathToShaders + "fragment.frag"));

	// Load Model
	ModelLoader modelLoader{ pathToModel + "sponza.obj" };

    const auto& meshes = modelLoader.GetMeshes();
	auto meshesSize = meshes.size();
	VAO.resize(meshesSize);
	VBO.resize(meshesSize);
	EBO.resize(meshesSize);
	TBO.resize(meshesSize);
	indicesSize.resize(meshesSize);

	for (unsigned int meshID = 0; meshID < meshesSize; ++meshID) {

		indicesSize[meshID] = meshes[meshID].GetIndices().size();

		// Vertex Array Object
		glGenVertexArrays(1, &VAO[meshID]);
		glBindVertexArray(VAO[meshID]);

		// Vertex Buffer Object for Cube
		glGenBuffers(1, &VBO[meshID]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[meshID]);
		glBufferData(GL_ARRAY_BUFFER, meshes[meshID].GetVertices().size() * sizeof(Vertex), &meshes[meshID].GetVertices().front(), GL_STATIC_DRAW);

		// Element Buffer Object for Cube
		glGenBuffers(1, &EBO[meshID]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[meshID]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshes[meshID].GetIndices().size() * sizeof(unsigned int), &meshes[meshID].GetIndices().front(), GL_STATIC_DRAW);

		// Set current VAO
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)0);
		glEnableVertexAttribArray(0);

		// Texture
		if (!meshes[meshID].GetTextures().empty()) {
			glGenTextures(1, &TBO[meshID]);
			glBindTexture(GL_TEXTURE_2D, TBO[meshID]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, meshes[meshID].GetTextures().at(0).GetWidth(), meshes[meshID].GetTextures().at(0).GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, meshes[meshID].GetTextures().at(0).GetTextureData());
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 8, (GLvoid*)(sizeof(GL_FLOAT) * 6));
		glEnableVertexAttribArray(1);
	}
    // Unbind current VAO
    glBindVertexArray(0);

    // Unbind current texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

static void uninitialize(void) 
{
	for (unsigned int meshID = 0; meshID < VAO.size(); ++meshID) {
		glDeleteVertexArrays(1, &VAO[meshID]);
		glDeleteBuffers(1, &VBO[meshID]);
		glDeleteBuffers(1, &EBO[meshID]);
	}

    glfwTerminate();
}

static void render(void)
{
    GLfloat angle = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader->UseProgram();
        
		for (unsigned int meshID = 0; meshID < VAO.size(); ++meshID) {
			glBindVertexArray(VAO[meshID]);

			// textures
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, TBO[meshID]);
			glUniform1i(glGetUniformLocation(shader->GetProgram(), "textureSampler"), 0);

			// transformations
			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -2.7f, -3.0f));
			glm::mat4 viewMatrix = camera.GetViewMatrix();
			glm::mat4 projectionMatrix = glm::perspective(45.0f, static_cast<float>(1024 / 768), 0.1f, 100.0f);

			// pass transformation to vertex shader
			GLint modelLocation = glGetUniformLocation(shader->GetProgram(), "model");
			GLint viewLocation = glGetUniformLocation(shader->GetProgram(), "view");
			GLint projectionLocation = glGetUniformLocation(shader->GetProgram(), "projection");

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			glDrawElements(GL_TRIANGLES, indicesSize[meshID], GL_UNSIGNED_INT, 0);
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