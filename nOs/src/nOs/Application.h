#pragma once

#include "Core.h"

#include "Window.h"
#include "nOs/LayerStack.h"
#include "nOs/Events/Event.h"
#include "nos/Events/ApplicationEvent.h"

#include "nOs/Renderer/Shader.h"
#include "nOs/Renderer/Buffer.h"


namespace nOs {
	class NOS_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }
		static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		//ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;

		LayerStack m_LayerStack;

		unsigned int m_VertexArray;
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

	private:
		static Application* s_Instance;
	};

	//To be defined in client
	Application* CreateApplication();
}
