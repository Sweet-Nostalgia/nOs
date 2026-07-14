#include "nospch.h"
#include "WindowsWindow.h"

#include "nOs/Events/ApplicationEvent.h"
#include "nOs/Events/KeyEvent.h"
#include "nOs/Events/MouseEvent.h"

namespace nOs {

	static bool s_PlatformWindowRegistered = false;

	//static void GLFWErrorCallback(int error, const char* description) {
	// NOS_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	// }

	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		m_Data.EventCallback = [](Event&) {};

		NOS_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_PlatformWindowRegistered) {
			WNDCLASSW wc = { };

			wc.lpfnWndProc = WindowsWindow::WindowProc;
			wc.hInstance = GetModuleHandleW(NULL);
			wc.lpszClassName = L"nOsEngineWindowClass";

			ATOM registerStatus = RegisterClassW(&wc);
			NOS_CORE_ASSERT(registerStatus, "Could not register Win32 class!");

			s_PlatformWindowRegistered = true;
		}

		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//NOS_CORE_ASSERT(status, "Could not initialize GLAD!");

		m_Window = CreateWindowExW(
			0,
			L"nOsEngineWindowClass",
			L"nOs Engine",
			WS_OVERLAPPEDWINDOW,

			CW_USEDEFAULT, CW_USEDEFAULT, m_Data.Width, m_Data.Height,

			NULL,
			NULL,
			GetModuleHandleW(NULL),
			this
		);

		//GLFWSetErrorCallback(GLFWErrorCallback);
		NOS_CORE_ASSERT(m_Window, "Could not create the Win32 Window!");

		ShowWindow(m_Window, SW_SHOW);
		SetVSync(true);
	}
	void WindowsWindow::Shutdown() {
		if (m_Window) {
			DestroyWindow(m_Window);
		}
	}

	void WindowsWindow::OnUpdate() {
		MSG msg;
		while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void WindowsWindow::SetVSync(bool enabled) {
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const {
		return m_Data.VSync;
	}

	LRESULT CALLBACK WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (uMsg == WM_NCCREATE) {
			CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
			SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(cs->lpCreateParams));
		}

		WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
		if (window) {
			switch (uMsg) {
			case WM_SIZE: {
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);
				window->m_Data.Width = width;
				window->m_Data.Height = height;
				WindowResizeEvent event(width, height);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_CLOSE: {
				WindowCloseEvent event;
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_DESTROY: {
				PostQuitMessage(0);
				return 0;
			}

			case WM_KEYDOWN: {
				int key = static_cast<int>(wParam);

				bool repeat = (lParam & (1 << 30)) != 0;

				KeyPressedEvent event(key, repeat ? 1 : 0);
				window->m_Data.EventCallback(event);

				return 0;
			}
			case WM_KEYUP: {
				int key = static_cast<int>(wParam);

				KeyReleasedEvent event(key);
				window->m_Data.EventCallback(event);

				return 0;
			}

			case WM_LBUTTONDOWN: {
				MouseButtonPressedEvent event(0);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_LBUTTONUP: {
				MouseButtonReleasedEvent event(0);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_RBUTTONDOWN: {
				MouseButtonPressedEvent event(1);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_RBUTTONUP: {
				MouseButtonReleasedEvent event(1);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_MOUSEWHEEL: {
				float yOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);

				MouseScrolledEvent event(0.0f, yOffset);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_MOUSEHWHEEL: { //This is for allowing horizontal scrolling
				float xOffset = static_cast<float>(GET_WHEEL_DELTA_WPARAM(wParam)) / static_cast<float>(WHEEL_DELTA);
				MouseScrolledEvent event(xOffset, 0.0f);
				window->m_Data.EventCallback(event);
				return 0;
			}
			case WM_MOUSEMOVE: {
				float xPos = static_cast<float>(LOWORD(lParam));
				float yPos = static_cast<float>(HIWORD(lParam));

				MouseMovedEvent event(xPos, yPos);
				window->m_Data.EventCallback(event);
				return 0;
			}
		}
	}
	return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}
}

