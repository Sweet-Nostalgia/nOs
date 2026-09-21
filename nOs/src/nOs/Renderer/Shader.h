#pragma once

#include <string>
#include <DirectXMath.h>
#include <d3d11.h>

namespace nOs {
    class Shader {
    public:
        Shader(const std::string& vertexSrc, const std::string& pixelSrc);
        ~Shader();

        void Bind() const;
        void Unbind() const;
        void SetUniformMat4(const std::string& name, const DirectX::XMMATRIX& matrix);

    private:
        ID3D11VertexShader* m_VertexShader = nullptr;
        ID3D11PixelShader* m_PixelShader = nullptr;
        ID3D11InputLayout* m_InputLayout = nullptr;
        ID3D11Buffer* m_ConstantBuffer = nullptr;
    };
}
