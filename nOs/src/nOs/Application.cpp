#include "nospch.h"
#include "Application.h"

#include "nOs/Log.h"
#include "Input.h"

#include "Platform/DirectX/DirectXContext.h"
#include "nOs/Renderer/Buffer.h"
#include "nOs/Renderer/Renderer.h"
#include "nOs/Renderer/RenderCommand.h"


namespace nOs {
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application* Application::s_Instance = nullptr;

	static DXGI_FORMAT ShaderDataTypeToDXGIFormat(ShaderDataType type) {
		switch (type) {
		case nOs::ShaderDataType::Float:	return DXGI_FORMAT_R32_FLOAT;
		case nOs::ShaderDataType::Float2:	return DXGI_FORMAT_R32G32_FLOAT;
		case nOs::ShaderDataType::Float3:	return DXGI_FORMAT_R32G32B32_FLOAT;
		case nOs::ShaderDataType::Float4:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case nOs::ShaderDataType::Int:		return DXGI_FORMAT_R32_SINT;
		case nOs::ShaderDataType::Int2:		return DXGI_FORMAT_R32G32_SINT;
		case nOs::ShaderDataType::Int3:		return DXGI_FORMAT_R32G32B32_SINT;
		case nOs::ShaderDataType::Int4:		return DXGI_FORMAT_R32G32B32A32_SINT;
		}
		NOS_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return DXGI_FORMAT_UNKNOWN;
	}

	Application::Application() {
		NOS_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		Renderer::Init(m_Window->GetGraphicsContext());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		//m_ImGuiLayer = new ImGuiLayer();
		//PushOverlay(m_ImGuiLayer);

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.0f,  0.5f, 0.0f
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" }
		};

		m_VertexBuffer->SetLayout(layout);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		
		m_Shader.reset(new Shader("D:\\Dev\\nOs\\bin\\Debug-x64\\Boost\\VertexShader.hlsl", "D:\\Dev\\nOs\\bin\\Debug-x64\\Boost\\PixelShader.hlsl"));
	}

	Application::~Application() {
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.IsHandled()) {
				break;
			}
		}
	}

	void Application::Run() {
		while (m_Running) {
			DirectXContext* dxContext = static_cast<DirectXContext*>(m_Window->GetGraphicsContext());

			RenderCommand::SetClearColor({0.6f, 0.4f, 0.8f, 1.0f});
			RenderCommand::Clear();
			
			Renderer::BeginScene();
			Renderer::Submit(m_Shader, m_VertexBuffer, m_IndexBuffer);
			Renderer::EndScene();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
			//m_ImGuiLayer->Begin();
			//for (Layer* layer : m_LayerStack)
			//	layer->OnImGuiRender();
			//m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}