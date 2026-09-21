#pragma once

#include "RenderCommand.h"

#include "nOs/Renderer/GraphicsContext.h"

namespace nOs {
    class Renderer {
    public:
        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader,
            const std::shared_ptr<VertexBuffer>& vertexBuffer,
            const std::shared_ptr<IndexBuffer>& indexBuffer);

        inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

        static void Init(GraphicsContext* context) { s_Context = context; }
        static GraphicsContext* GetContext() { return s_Context; }

    private:
        static GraphicsContext* s_Context;
    };
}
