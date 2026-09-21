#pragma once

#include "nOs/Window.h"
#include "nOs/Renderer/GraphicsContext.h"

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

		inline virtual void* GetNativeWindow() const {
			return m_Window;
		}

		//TODO: Remove this when the renderer abstraction is built
		GraphicsContext* GetGraphicsContext() const{
			return m_Context;
		}

	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		HWND m_Window;
		GraphicsContext* m_Context;

		struct WindowData {
			std::string Title = "nOs Engine";
			unsigned int Width = 1280;
			unsigned int Height = 720;
			bool VSync = false;

			EventCallbackFn EventCallback = [](Event&) {};
		};

		WindowData m_Data;
	};
}