#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <memory>
#include <windows.h>

namespace WindowSystem {

struct WindowHandle {
	union {
		HWND winApiHandle;
	};
};

struct WindowProperties {
	unsigned int width;
	unsigned int height;
};

class Window{
public:
	Window() = delete;
	Window(const WindowProperties& properties);
	Window(const Window&) = delete;
	Window(Window&&) = delete;
    ~Window() = default;

	virtual bool CreateWindow_() = 0;
	virtual bool DestroyWindow_() = 0;
	virtual bool ShowWindow_() = 0;
	const WindowHandle GetWindowHandle() const;

protected:
	WindowProperties m_properties;
	WindowHandle m_handle;
};

class WindowWin32 : public Window {
public:
	WindowWin32() = delete;
	WindowWin32(const WindowProperties& properties);
	WindowWin32(const WindowWin32&) = delete;
	WindowWin32(WindowWin32&&) = delete;
	~WindowWin32() = default;

	virtual bool CreateWindow_();
	virtual bool DestroyWindow_();
	virtual bool ShowWindow_();
private:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

class WindowXlib : public Window {
public:
	WindowXlib() = default;
	WindowXlib(const WindowProperties& properties);
	WindowXlib(const WindowXlib&) = delete;
	WindowXlib(WindowXlib&&) = delete;
	~WindowXlib() = default;

	virtual bool CreateWindow_();
	virtual bool DestroyWindow_();
	virtual bool ShowWindow_();
};

class WindowManager {
public:

	enum class WindowSystem {
		WINDOWS, LINUX
	};

	std::shared_ptr<Window> GetWindow(WindowSystem, const WindowProperties&);
};

}
#endif // !WINDOW_HPP
