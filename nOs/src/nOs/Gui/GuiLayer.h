#pragma once

#include "nOs/Layer.h"
#include "render/GuiRenderer.h"

namespace nOs {
	class NOS_API GuiLayer : public Layer {
	public:
		GuiLayer();
		~GuiLayer();
		
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& e) override;

	private:
		GuiRenderer m_Renderer;
	};

}