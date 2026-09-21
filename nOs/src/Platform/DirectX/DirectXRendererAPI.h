#pragma once

#include "nOs/Renderer/RendererAPI.h"

namespace nOs {
	class DirectXRendererAPI : public RendererAPI {
	public:
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexBuffer>& vertexBuffer,
			const std::shared_ptr<IndexBuffer>& indexBuffer) override;
	private:
		DirectX::XMFLOAT4 m_ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}