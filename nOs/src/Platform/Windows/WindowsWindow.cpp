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

		UINT createDeviceFlags = 0;
		#if defined(DEBUG) || defined(_DEBUG)
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
		#endif

		D3D_FEATURE_LEVEL featureLevel;
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			createDeviceFlags,
			nullptr, 0,
			D3D11_SDK_VERSION,
			device.GetAddressOf(),
			&featureLevel,
			context.GetAddressOf()
		);

		NOS_CORE_ASSERT(SUCCEEDED(hr), "D3D11CreateDevice failed!");

		NOS_CORE_ASSERT(featureLevel == D3D_FEATURE_LEVEL_11_0, "D3D11CreateDevice failed!");

		//Describing the Swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = m_Data.Width;
		sd.BufferDesc.Height = m_Data.Height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_Window;
		sd.Windowed = true;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = 0;

		//Creating the swap chain
		Microsoft::WRL::ComPtr<IDXGIDevice> dxgiDevice;
		NOS_CORE_ASSERT(SUCCEEDED(device->QueryInterface(__uuidof(IDXGIDevice),
			(void**)dxgiDevice.GetAddressOf())), "Failed to get IDXGIDevice!");

		Microsoft::WRL::ComPtr<IDXGIAdapter> dxgiAdapter;
		NOS_CORE_ASSERT(SUCCEEDED(dxgiDevice->GetParent(__uuidof(IDXGIAdapter),
			(void**)dxgiAdapter.GetAddressOf())), "Failed to get IDXGIAdapter!");

		Microsoft::WRL::ComPtr<IDXGIFactory> dxgiFactory;
		NOS_CORE_ASSERT(SUCCEEDED(dxgiAdapter->GetParent(__uuidof(IDXGIFactory),
			(void**)dxgiFactory.GetAddressOf())), "Failed to get IDXGIFactory!");

		NOS_CORE_ASSERT(SUCCEEDED(dxgiFactory->CreateSwapChain(device.Get(), &sd,
			swapChain.GetAddressOf())), "Failed to create swap chain!");

		//Render Target View
		Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)backBuffer.GetAddressOf());
		device->CreateRenderTargetView(backBuffer.Get(), 0, renderTargetView.GetAddressOf());

		//Depth/Stencil Buffer & View
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Width = m_Data.Width;
		depthStencilDesc.Height = m_Data.Height;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;

		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		device->CreateTexture2D(&depthStencilDesc, 0, depthStencilBuffer.GetAddressOf());
		device->CreateDepthStencilView(depthStencilBuffer.Get(), 0, depthStencilView.GetAddressOf());

		//Binding Views to Output Merger State
		ID3D11RenderTargetView* rtv[] = {renderTargetView.Get()};
		context->OMSetRenderTargets(1, rtv, depthStencilView.Get());

		//Set the Viewport
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(m_Data.Width);
		vp.Height = static_cast<float>(m_Data.Height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		context->RSSetViewports(1, &vp);

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

		float clearColor[4] = { 0.6f, 0.4f, 0.8f, 1.0f };
		context->ClearRenderTargetView(renderTargetView.Get(), clearColor);
		context->ClearDepthStencilView(depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		swapChain->Present(0, 0);
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