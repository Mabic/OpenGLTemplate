#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <functional>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "Application.hpp"
#include "ModelLoader.hpp"
#include "Light.hpp"
#include "Object.hpp"
#include "Shader.h"
#include "TransformationMaterialUBO.hpp"

namespace {
	const std::string pathToProject(PROJECT_PATH);

	const std::string pathToShaders(pathToProject + "/Applications/Scene/Resources/Shaders/");
	const std::string pathToTextures(pathToProject + "/Applications/Scene/Resources/Textures/");
	const std::string pathToModel(pathToProject + "/Applications/Scene/Resources/Models/");
}

void Application::GLFWCallbackWrapper::MousePositionCallback(GLFWwindow* window, double positionX, double positionY)
{
	s_application->MousePositionCallback(window, positionX, positionY);
}

void Application::GLFWCallbackWrapper::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	s_application->KeyboardCallback(window, key, scancode, action, mods);
	ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
}

void Application::GLFWCallbackWrapper::SetApplication(Application* application)
{
	GLFWCallbackWrapper::s_application = application;
}

Application* Application::GLFWCallbackWrapper::s_application = nullptr;

Application::Application(int windowWidth, int windowHeight)
	: m_window(nullptr),
	  m_windowWidth(windowWidth),
	  m_windowHeight(windowHeight),
	  m_pressedKey{false}
{}

Application::~Application()
{
	glfwTerminate();
}

void Application::Initialize()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, "Hello Triangle", NULL, NULL);

	if (!m_window) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return;
	}

	glfwMakeContextCurrent(m_window);

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(m_window, false);

	SetCallbackFunctions();
	SetInput();

	glewExperimental = GL_TRUE;
	glewInit();

	// to reset error flags after glewInit
	glGetError();

	glViewport(0, 0, m_windowWidth, m_windowHeight);
	glEnable(GL_DEPTH_TEST);

	m_uniformBuffer.reset(new TransformationMaterialBuffer());

	SetUpObjects();
	SetUpLight();
}

void Application::Render()
{
	float angle = 0.0f;

	while (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_shader->UseProgram();
		glfwPollEvents();
		CameraMovement();
		angle += 0.000f;

		for (Object& object : m_objects) {

			if (!object.IsMeshRenderable()) {
				continue;
			}

			// transformations
			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, -0.0f, -6.0f));
			modelMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0.0f, 1.0f, 0.0f));
			glm::mat4 viewMatrix = m_camera.GetViewMatrix();
			glm::mat4 projectionMatrix = glm::perspective(45.0f, static_cast<float>(m_windowWidth / m_windowHeight), 0.1f, 100.0f);

			// pass transformation to vertex shader
			GLint modelLocation = glGetUniformLocation(m_shader->GetProgram(), "model");
			GLint viewLocation = glGetUniformLocation(m_shader->GetProgram(), "view");
			GLint projectionLocation = glGetUniformLocation(m_shader->GetProgram(), "projection");
			GLint cameraLocation = glGetUniformLocation(m_shader->GetProgram(), "cameraPosition");

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
			glUniform3fv(cameraLocation, 1, &m_camera.GetPosition()[0]);

			object.Render(*m_shader);
		}

		{
			m_lightningShader->UseProgram();

			// transformations
			glm::mat4 modelMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(m_lights[0].GetData().m_position));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
			glm::mat4 viewMatrix = m_camera.GetViewMatrix();
			glm::mat4 projectionMatrix = glm::perspective(45.0f, static_cast<float>(m_windowWidth / m_windowHeight), 0.1f, 100.0f);

			// pass transformation to vertex shader
			GLint modelLocation = glGetUniformLocation(m_lightningShader->GetProgram(), "model");
			GLint viewLocation = glGetUniformLocation(m_lightningShader->GetProgram(), "view");
			GLint projectionLocation = glGetUniformLocation(m_lightningShader->GetProgram(), "projection");

			glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
			glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

			m_lights[0].Render();
		}

		
		if (glfwGetInputMode(m_window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
		{
			ImGui_ImplGlfwGL3_NewFrame();

			static float f = 0.0f;
			ImGui::Text("Hello, world!");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::Render();
		}

		

		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}
}

void Application::CleanUp()
{
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
}

void Application::SetCallbackFunctions()
{
	GLFWCallbackWrapper::SetApplication(this);
	glfwSetCursorPosCallback(m_window, GLFWCallbackWrapper::MousePositionCallback);
	glfwSetKeyCallback(m_window, GLFWCallbackWrapper::KeyboardCallback);

	glfwSetMouseButtonCallback(m_window, ImGui_ImplGlfwGL3_MouseButtonCallback);
	glfwSetScrollCallback(m_window, ImGui_ImplGlfwGL3_ScrollCallback);
	glfwSetCharCallback(m_window, ImGui_ImplGlfwGL3_CharCallback);
}

void Application::SetInput()
{
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);
}

void Application::MousePositionCallback(GLFWwindow* window, double positionX, double positionY)
{
	int cursorMode = glfwGetInputMode(window, GLFW_CURSOR);

	if (cursorMode != GLFW_CURSOR_DISABLED) {
		return;
	}

	if (m_firstMouse)
	{
		m_lastPositionX = static_cast<GLfloat>(positionX);
		m_lastPositionY = static_cast<GLfloat>(positionY);
		m_firstMouse = false;
	}
	GLfloat xoffset = static_cast<GLfloat>(positionX) - m_lastPositionX;
	GLfloat yoffset = m_lastPositionY - static_cast<GLfloat>(positionY);

	m_lastPositionX = static_cast<GLfloat>(positionX);
	m_lastPositionY = static_cast<GLfloat>(positionY);

	m_camera.UpdateEulerAngles(xoffset, yoffset);
}

void Application::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_pressedKey[key] = true;
	else if (action == GLFW_RELEASE)
		m_pressedKey[key] = false;

	if (key == GLFW_KEY_F && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void Application::CameraMovement()
{
	if (m_pressedKey[GLFW_KEY_W]) {
		m_camera.UpdatePosition(Camera::DIRECTION::FORWARD);
	}

	if (m_pressedKey[GLFW_KEY_S]) {
		m_camera.UpdatePosition(Camera::DIRECTION::BACKWARD);
	}

	if (m_pressedKey[GLFW_KEY_A]) {
		m_camera.UpdatePosition(Camera::DIRECTION::LEFT);
	}

	if (m_pressedKey[GLFW_KEY_D]) {
		m_camera.UpdatePosition(Camera::DIRECTION::RIGHT);
	}
}

void Application::SetUpLight()
{
	ModelLoader lightModel(pathToModel + "cube/cube.obj");
	LightData light(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	m_lightningShader.reset(new Shader(pathToShaders + "light.vert", pathToShaders + "light.frag"));
	m_uniformBuffer->UpdateLight(light);
	m_lights.push_back(Light(std::move(light), lightModel.GetMeshes().front()));
}

void Application::SetUpObjects()
{
	ModelLoader modelLoader(pathToModel + "lost_empire/lost_empire.obj");
	const auto& meshes = modelLoader.GetMeshes();

	m_shader.reset(new Shader(pathToShaders + "vertex.vert", pathToShaders + "fragment.frag"));
	m_uniformBuffer->Bind(m_shader.get());
	m_uniformBuffer->UpdateMaterial(meshes[0].GetMaterial());

	for (const Mesh& mesh : meshes) {
		m_objects.push_back(Object(mesh));
	}
}