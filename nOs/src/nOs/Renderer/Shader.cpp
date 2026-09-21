#include "nospch.h"
#include "Shader.h"
#include "Renderer.h"
#include "Platform/DirectX/DirectXContext.h"


namespace nOs {

    Shader::Shader(const std::string& vertexSrc, const std::string& pixelSrc) {
        DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
        ID3DBlob* vsBlob = nullptr;
        ID3DBlob* psBlob = nullptr;
        ID3D10Blob* errorBlob = nullptr;

        HRESULT hr = D3DCompileFromFile(
            std::wstring(vertexSrc.begin(), vertexSrc.end()).c_str(),
            nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, &errorBlob);

        if (FAILED(hr)) {
            if (errorBlob) {
                NOS_CORE_ERROR("Vertex shader compile error: {0}", (char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
            }
            NOS_CORE_ASSERT(false, "Vertex shader compilation failed!");
        }

        dxContext->GetDevice()->CreateVertexShader(vsBlob->GetBufferPointer(),
            vsBlob->GetBufferSize(),
            nullptr, &m_VertexShader);

        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,
              D3D11_INPUT_PER_VERTEX_DATA, 0 }
        };

        dxContext->GetDevice()->CreateInputLayout(layout, ARRAYSIZE(layout),
            vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &m_InputLayout);

        hr = D3DCompileFromFile(
            std::wstring(pixelSrc.begin(), pixelSrc.end()).c_str(),
            nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, &errorBlob);

        if (FAILED(hr)) {
            if (errorBlob) {
                NOS_CORE_ERROR("Pixel shader compile error: {0}", (char*)errorBlob->GetBufferPointer());
                errorBlob->Release();
                errorBlob = nullptr;
            }
            NOS_CORE_ASSERT(false, "Pixel shader compilation failed!");
        }

        dxContext->GetDevice()->CreatePixelShader(psBlob->GetBufferPointer(),
            psBlob->GetBufferSize(),
            nullptr, &m_PixelShader);

        if (vsBlob) vsBlob->Release();
        if (psBlob) psBlob->Release();
    }

    Shader::~Shader() {
        if (m_VertexShader) m_VertexShader->Release();
        if (m_PixelShader) m_PixelShader->Release();
        if (m_InputLayout) m_InputLayout->Release();
    }

    void Shader::Bind() const {
        DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
        dxContext->GetContext()->IASetInputLayout(m_InputLayout);
        dxContext->GetContext()->VSSetShader(m_VertexShader, nullptr, 0);
        dxContext->GetContext()->PSSetShader(m_PixelShader, nullptr, 0);
    }

    void Shader::Unbind() const {
        DirectXContext* dxContext = static_cast<DirectXContext*>(Renderer::GetContext());
        dxContext->GetContext()->IASetInputLayout(nullptr);
        dxContext->GetContext()->VSSetShader(nullptr, nullptr, 0);
        dxContext->GetContext()->PSSetShader(nullptr, nullptr, 0);
    }
}