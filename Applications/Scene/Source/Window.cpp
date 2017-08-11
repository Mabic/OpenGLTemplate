#include "Window.hpp"

namespace WindowSystem {

Window::Window(const WindowProperties& properties)
	: m_properties(properties)
{

}

const WindowHandle Window::GetWindowHandle() const
{
	return m_handle;
}

WindowWin32::WindowWin32(const WindowProperties& properties)
	: Window(properties)
{
}

bool WindowWin32::CreateWindow_()
{
	HMODULE HIn = GetModuleHandle(NULL);

	WNDCLASSEX wndClass;
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = 0;
	wndClass.lpfnWndProc = WindowWin32::WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = HIn;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "WindowWin32";
	wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wndClass)) {
		return false;
	}

	// Create the window.
	m_handle.winApiHandle = CreateWindowEx(0,					// Optional window styles.
										   "WindowWin32",		// Window class
										   "Window",			// Window text
										   WS_OVERLAPPEDWINDOW, // Window style
										   0,					// start x
										   0,					// start y
										   m_properties.width,  // width
										   m_properties.height,	// height	
										   NULL,				// Parent window    
										   NULL,				// Menu
										   HIn,					// Instance handle
										   NULL					// Additional application data
	);

	if (m_handle.winApiHandle == nullptr) {
		return false;
	}

	return true;
}

bool WindowWin32::DestroyWindow_()
{
	return DestroyWindow(m_handle.winApiHandle);
}

bool WindowWin32::ShowWindow_()
{
	return ShowWindow(m_handle.winApiHandle, SW_SHOW);
}

LRESULT CALLBACK WindowWin32::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hwnd);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

WindowXlib::WindowXlib(const WindowProperties& properties)
	: Window(properties)
{

}

bool WindowXlib::CreateWindow_()
{
	return true;
}

bool WindowXlib::DestroyWindow_()
{
	return true;
}

bool WindowXlib::ShowWindow_()
{
	return true;
}

std::shared_ptr<Window> WindowManager::GetWindow(WindowManager::WindowSystem system, const WindowProperties& properties)
{
	switch (system)
	{
	case WindowManager::WindowSystem::LINUX:
		return std::make_shared<WindowXlib>(properties);
	case WindowManager::WindowSystem::WINDOWS:
		return std::make_shared<WindowWin32>(properties);
	default:
		exit(0);
	}
}

}