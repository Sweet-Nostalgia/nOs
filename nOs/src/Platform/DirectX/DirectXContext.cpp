#include "nospch.h"
#include "DirectXContext.h"

namespace nOs {
	DirectXContext::DirectXContext(HWND windowHandle)
		: m_WindowHandle(windowHandle), m_VSync(false) {
		NOS_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}
	void DirectXContext::Init() {
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

		RECT rect;
		GetClientRect(m_WindowHandle, &rect);
		UINT width = rect.right - rect.left;
		UINT height = rect.bottom - rect.top;

		//Describing the Swap chain
		DXGI_SWAP_CHAIN_DESC sd;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = m_WindowHandle;
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
		depthStencilDesc.Width = width;
		depthStencilDesc.Height = height;
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
		ID3D11RenderTargetView* rtv[] = { renderTargetView.Get() };
		context->OMSetRenderTargets(1, rtv, depthStencilView.Get());

		//Set the Viewport
		D3D11_VIEWPORT vp;
		vp.TopLeftX = 0.0f;
		vp.TopLeftY = 0.0f;
		vp.Width = static_cast<float>(width);
		vp.Height = static_cast<float>(height);
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;

		context->RSSetViewports(1, &vp);

		D3D11_RASTERIZER_DESC rasterDesc = {};
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FrontCounterClockwise = FALSE;
		rasterDesc.DepthClipEnable = TRUE;

		device->CreateRasterizerState(&rasterDesc, rasterState.GetAddressOf());
		context->RSSetState(rasterState.Get());

		//NOS_CORE_INFO("DirectX Info:")
		//NOS_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//NOS_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//NOS_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void DirectXContext::SwapBuffers() {
		UINT syncInterval = m_VSync ? 1 : 0;
		swapChain->Present(syncInterval, 0);
	}
}