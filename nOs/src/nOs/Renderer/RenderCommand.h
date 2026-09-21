#pragma once

#include "RendererAPI.h"

namespace nOs {
	class RenderCommand {
	public:
		inline static void SetClearColor(const DirectX::XMFLOAT4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		inline static void Clear() {
			s_RendererAPI->Clear();
		}

		inline static void DrawIndexed(const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexBuffer>& vertexBuffer,
			const std::shared_ptr<IndexBuffer>& indexBuffer) {
			s_RendererAPI->DrawIndexed(shader, vertexBuffer, indexBuffer);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}