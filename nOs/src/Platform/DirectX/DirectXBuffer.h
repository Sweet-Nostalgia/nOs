#pragma once

#include "nOs/Renderer/Buffer.h"

namespace nOs {
	class DirectXVertexBuffer : public VertexBuffer {
	public:
		DirectXVertexBuffer(float* vertices, uint32_t size);
		virtual ~DirectXVertexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; };
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

	private:
		ID3D11Buffer* m_Buffer;
		BufferLayout m_Layout;
	};


	class DirectXIndexBuffer : public IndexBuffer {
	public:
		DirectXIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~DirectXIndexBuffer();

		void Bind() const override;
		void Unbind() const override;

		virtual uint32_t GetCount() const { return m_Count; }

	private:
		ID3D11Buffer* m_Buffer;
		uint32_t m_Count;
	};
}
