#pragma once

#include <DirectXMath.h>

#include "nOs/Renderer/Buffer.h"
#include "nOs/Renderer/Shader.h"

namespace nOs {
	class RendererAPI {
	public:
		enum class API {
			None = 0, DirectX = 1
		};
	public:
		virtual void SetClearColor(const DirectX::XMFLOAT4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const std::shared_ptr<Shader>& shader,
			const std::shared_ptr<VertexBuffer>& vertexBuffer,
			const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

		inline static API GetAPI() { return s_API; };
	private:
		static API s_API;
	};
}