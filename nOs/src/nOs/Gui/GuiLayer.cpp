#include "nospch.h"
#include "GuiLayer.h"

#include "nOs/Application.h"

namespace nOs {
	GuiLayer::GuiLayer()
		: Layer("GuiLayer") {
	}
	GuiLayer::~GuiLayer() {
	}

	void GuiLayer::OnAttach() {
		Window& window = Application::Get().GetWindow();

		ID3D11Device* device = static_cast<ID3D11Device*>(window.GetDevice());
		ID3D11DeviceContext* context = static_cast<ID3D11DeviceContext*>(window.GetContext());

		m_Renderer.Init(device, context, window.GetWidth(), window.GetHeight());
	}

	void GuiLayer::OnDetach() {
		m_Renderer.Shutdown();
	}

	void GuiLayer::OnUpdate() {
		m_Renderer.DrawRect(100.0f, 100.0f, 200.0f, 100.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	}

	void GuiLayer::OnEvent(Event& e) {
	}
}