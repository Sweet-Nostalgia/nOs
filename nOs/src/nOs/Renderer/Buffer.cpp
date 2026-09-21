#include "nospch.h"
#include "Buffer.h"

#include "Renderer.h"

#include "Platform/DirectX/DirectXBuffer.h"

namespace nOs {
    VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            NOS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::API::DirectX:
            return new DirectXVertexBuffer(vertices, size);
        }
        NOS_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }

    IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None:
            NOS_CORE_ASSERT(false, "RendererAPI::None is currently not supported");
            return nullptr;
        case RendererAPI::API::DirectX:
            return new DirectXIndexBuffer(indices, count);
        }
        NOS_CORE_ASSERT(false, "Unknown RendererAPI");
        return nullptr;
    }
}