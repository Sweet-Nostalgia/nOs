#include "nospch.h"
#include "DirectXBuffer.h"
#include "DirectXContext.h"
#include "nOs/Renderer/Renderer.h"

namespace nOs {
	DirectXVertexBuffer::DirectXVertexBuffer(float* vertices, uint32_t size) {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());

		D3D11_BUFFER_DESC vbd = {};
		vbd.Usage = D3D11_USAGE_IMMUTABLE;
		vbd.ByteWidth = size * sizeof(float);
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vinitData = {};
		vinitData.pSysMem = vertices;

		dxContext->GetDevice()->CreateBuffer(&vbd, &vinitData, &m_Buffer);
	}

	DirectXVertexBuffer::~DirectXVertexBuffer() {
		if (m_Buffer) m_Buffer->Release();
	}

	void DirectXVertexBuffer::Bind() const {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
		UINT stride = m_Layout.GetStride();
		UINT offset = 0;
		dxContext->GetContext()->IASetVertexBuffers(0, 1, &m_Buffer, &stride, &offset);
	}

	void DirectXVertexBuffer::Unbind() const {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
		ID3D11Buffer* nullBuffer = nullptr;
		UINT stride = 0;
		UINT offset = 0;
		dxContext->GetContext()->IASetVertexBuffers(0, 1, &nullBuffer, &stride, &offset);
	}

	DirectXIndexBuffer::DirectXIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count) {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
		D3D11_BUFFER_DESC ibd = {};
		ibd.Usage = D3D11_USAGE_IMMUTABLE;
		ibd.ByteWidth = count * sizeof(uint32_t);
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA iinitData = {};
		iinitData.pSysMem = indices;

		dxContext->GetDevice()->CreateBuffer(&ibd, &iinitData, &m_Buffer);
	}

	DirectXIndexBuffer::~DirectXIndexBuffer() {
		if (m_Buffer) m_Buffer->Release();
	}

	void DirectXIndexBuffer::Bind() const {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
		dxContext->GetContext()->IASetIndexBuffer(m_Buffer, DXGI_FORMAT_R32_UINT, 0);
	}

	void DirectXIndexBuffer::Unbind() const {
		DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
		dxContext->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	}
}