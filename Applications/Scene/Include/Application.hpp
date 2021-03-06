#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <array>
#include <memory>
#include <vector>

#include "Camera.hpp"

struct GLFWwindow;
class Light;
class Object;
class Shader;
class TransformationMaterialBuffer;

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

	void SetUpLight();
	void SetUpObjects();

	// window definitions
	GLFWwindow* m_window;
	int m_windowWidth;
	int m_windowHeight;

	// shaders
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Shader> m_lightningShader;

	// uniforms
	std::unique_ptr<TransformationMaterialBuffer> m_uniformBuffer;

	// meshes
	std::vector<Object> m_objects;

	// lights
	std::vector<Light> m_lights;

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
