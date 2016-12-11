#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <AntTweakBar.h>

#include <array>
#include <memory>
#include <vector>

#include "Camera.hpp"

struct GLFWwindow;
class Shader;

class Application
{
public:
	Application() = delete;
	Application(int windowWidth, int windowHeight);
	Application(const Application&) = delete;
	Application(Application&&) = delete;
	~Application();

	void Initialize();
	void Render();
	void CleanUp();
private:
	void SetCallbackFunctions();
	void SetInput();
	void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);
	void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void CameraMovement();

	// window definitions
	GLFWwindow* m_window;
	int m_windowWidth;
	int m_windowHeight;

	// shaders
	std::unique_ptr<Shader> m_shader;

	// buffer objects
	std::vector<unsigned int> m_vertexArrayObject;
	std::vector<unsigned int> m_vertexBufferObject;
	std::vector<unsigned int> m_elementBufferObject;
	std::vector<unsigned int> m_textureBufferObject;
	
	// meshes
	std::vector<short> m_activeMeshes;
	std::vector<size_t> m_indicesSize;

	// TweakBar
	TwBar* m_TweakBar;

	// camera
	Camera m_camera;
	float m_lastPositionX;
	float m_lastPositionY;
	bool m_firstMouse;
	std::array<bool, 1024> m_pressedKey;

	// since GLFW callbacks are C API, need helper wrapper for this
	class GLFWCallbackWrapper
	{
	public:
		GLFWCallbackWrapper() = delete;
		GLFWCallbackWrapper(const GLFWCallbackWrapper&) = delete;
		GLFWCallbackWrapper(GLFWCallbackWrapper&&) = delete;
		~GLFWCallbackWrapper() = delete;

		static void MousePositionCallback(GLFWwindow* window, double positionX, double positionY);
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void SetApplication(Application *application);
	private:
		static Application* s_application;
	};
};

#endif // !APPLICATION_HPP