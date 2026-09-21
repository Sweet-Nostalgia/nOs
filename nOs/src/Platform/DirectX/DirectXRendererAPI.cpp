#include "nospch.h"
#include "DirectXRendererAPI.h"
#include "DirectXContext.h"

#include "nOs/Renderer/Buffer.h"
#include "nOs/Renderer/Renderer.h"
#include "nOs/Renderer/Shader.h"

#include <d3d11.h>

namespace nOs {
	void DirectXRendererAPI::SetClearColor(const DirectX::XMFLOAT4& color) {
		m_ClearColor = color;
	}
	void DirectXRendererAPI::Clear() {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());

		float clearColor[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
		dxContext->GetContext()->ClearRenderTargetView(dxContext->GetRenderTargetView(), clearColor);
		dxContext->GetContext()->ClearDepthStencilView(dxContext->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	}

	void DirectXRendererAPI::DrawIndexed(const std::shared_ptr<Shader>& shader,
		const std::shared_ptr<VertexBuffer>& vertexBuffer,
		const std::shared_ptr<IndexBuffer>& indexBuffer) {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());

		shader->Bind();
		vertexBuffer->Bind();
		indexBuffer->Bind();

		dxContext->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		dxContext->GetContext()->DrawIndexed(indexBuffer->GetCount(), 0, 0);
	}
}