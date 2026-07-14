#pragma once

#include "nOspch.h"

#include "nOs/Core.h"
#include "nOs/Events/Event.h"

namespace nOs {
	struct WindowProps {
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "nOs Engine",
					UINT width = 1280,
					UINT height = 720)
			: Title(title), Width(width), Height(height) {
		}
	};

	class NOS_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}