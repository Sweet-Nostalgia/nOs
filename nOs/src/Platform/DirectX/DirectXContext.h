#pragma once
#include "nOS/Renderer/GraphicsContext.h"
#include "nospch.h"

namespace nOs {
	class DirectXContext : public GraphicsContext {
	public:
		DirectXContext(HWND windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
		void SetVSync(bool enabled);
		bool IsVSync() const;

		//TEMPORARY
		//TODO: Remove this when the DirectXContext is fully implemented
		ID3D11Device* GetDevice() const { return device.Get(); }
		ID3D11DeviceContext* GetContext() const { return context.Get(); }
		IDXGISwapChain* GetSwapChain() const { return swapChain.Get(); }
		ID3D11RenderTargetView* GetRenderTargetView() const { return renderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencilView() const { return depthStencilView.Get(); }

	private:
		HWND m_WindowHandle;
		bool m_VSync;

		Microsoft::WRL::ComPtr<ID3D11Device>			device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterState;
	};
}