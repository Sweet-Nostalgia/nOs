#pragma once

#include "Core.h"

#include "Window.h"
#include "nOs/LayerStack.h"
#include "nOs/Events/Event.h"
#include "nos/Events/ApplicationEvent.h"


namespace nOs {
	class NOS_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;

		LayerStack m_LayerStack;
	};

	//To be defined in client
	Application* CreateApplication();
}
