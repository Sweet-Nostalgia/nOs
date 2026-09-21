#include "nospch.h"
#include "Renderer.h"

namespace nOs {

    void Renderer::BeginScene() {

    }
    void Renderer::EndScene() {

    }

    void Renderer::Submit(const std::shared_ptr<Shader>& shader,
        const std::shared_ptr<VertexBuffer>& vertexBuffer,
        const std::shared_ptr<IndexBuffer>& indexBuffer) {

        RenderCommand::DrawIndexed(shader, vertexBuffer, indexBuffer);
    }
    GraphicsContext* Renderer::s_Context = nullptr;
}
