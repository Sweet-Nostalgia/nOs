#pragma once

#include "nOs/Window.h"

#include "nospch.h"

namespace nOs {
	class WindowsWindow : public Window {
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override {
			return m_Data.Width;
		}
		inline unsigned int GetHeight() const override {
			return m_Data.Height;
		}

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override {
			m_Data.EventCallback = callback;
		}

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_Window;

		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;

		Microsoft::WRL::ComPtr<ID3D11Device>			device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		context;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			swapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	};
}