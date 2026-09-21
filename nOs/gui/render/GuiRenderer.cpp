#include "nospch.h"
#include "GuiRenderer.h"

namespace nOs {

	static const char* s_ShaderSource = R"(
		cbuffer ProjectionBuffer : register(b0)
		{
			matrix projection;
		};

		struct VS_INPUT
		{
			float2 pos		: POSITION;
			float4 color	: COLOR;
		};

		struct PS_INPUT
		{
			float4 pos		: SV_POSITION;
			float4 color	: COLOR;
		};

		PS_INPUT VSMain(VS_INPUT input)
		{
			PS_INPUT output;
			output.pos = mul(float4(input.pos, 0.0f, 1.0f), projection);
			output.color = input.color;
			return output;
		}

		float4 PSMain(PS_INPUT input) : SV_TARGET
		{
			return input.color;
		}
	)";

	struct ProjectionBuffer {
		DirectX::XMMATRIX projection;
	};

	bool GuiRenderer::Init(ID3D11Device* device, ID3D11DeviceContext* context, uint32_t screenWidth, uint32_t screenHeight) {
		m_Device = device;
		m_Context = context;

		m_ProjMatrix = DirectX::XMMatrixOrthographicOffCenterLH(
			0.0f, static_cast<float>(screenWidth),
			static_cast<float>(screenHeight), 0.0f,
			0.0f, 1.0f
		);

		Microsoft::WRL::ComPtr<ID3DBlob> vsBlob, psBlob, errorBlob;

		HRESULT hr = D3DCompile(s_ShaderSource, strlen(s_ShaderSource), nullptr, nullptr, nullptr,
			"VSMain", "vs_5_0", 0, 0, vsBlob.GetAddressOf(), errorBlob.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), errorBlob ? (char*)errorBlob->GetBufferPointer() : "Unknown VS compile error");

		hr = D3DCompile(s_ShaderSource, strlen(s_ShaderSource), nullptr, nullptr, nullptr,
			"PSMain", "ps_5_0", 0, 0, psBlob.GetAddressOf(), errorBlob.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), errorBlob ? (char*)errorBlob->GetBufferPointer() : "Unknown PS compile error");

		hr = m_Device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex shader!");

		hr = m_Device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create pixel shader!");

		D3D11_INPUT_ELEMENT_DESC layout[] = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,       0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		hr = m_Device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create input layout!");

		D3D11_BUFFER_DESC vbDesc = {};
		vbDesc.Usage = D3D11_USAGE_DYNAMIC;
		vbDesc.ByteWidth = sizeof(Vertex) * 6;
		vbDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = m_Device->CreateBuffer(&vbDesc, nullptr, m_VertexBuffer.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer!");

		D3D11_BUFFER_DESC cbDesc = {};
		cbDesc.Usage = D3D11_USAGE_DYNAMIC;
		cbDesc.ByteWidth = sizeof(ProjectionBuffer);
		cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = m_Device->CreateBuffer(&cbDesc, nullptr, m_ConstantBuffer.GetAddressOf());
		NOS_CORE_ASSERT(SUCCEEDED(hr), "Failed to create constant buffer!");

		return true;
	}

	void GuiRenderer::DrawRect(float x, float y, float w, float h, float r, float g, float b, float a) {
		Vertex verts[6] = {
			{ x,     y,     r, g, b, a },
			{ x + w, y,     r, g, b, a },
			{ x,     y + h, r, g, b, a },

			{ x + w, y,     r, g, b, a },
			{ x + w, y + h, r, g, b, a },
			{ x,     y + h, r, g, b, a },
		};

		D3D11_MAPPED_SUBRESOURCE mapped;
		m_Context->Map(m_VertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		memcpy(mapped.pData, verts, sizeof(verts));
		m_Context->Unmap(m_VertexBuffer.Get(), 0);

		D3D11_MAPPED_SUBRESOURCE cbMapped;
		m_Context->Map(m_ConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &cbMapped);
		ProjectionBuffer pb{ DirectX::XMMatrixTranspose(m_ProjMatrix) };
		memcpy(cbMapped.pData, &pb, sizeof(pb));
		m_Context->Unmap(m_ConstantBuffer.Get(), 0);

		UINT stride = sizeof(Vertex), offset = 0;
		m_Context->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		m_Context->IASetInputLayout(m_InputLayout.Get());
		m_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_Context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
		m_Context->VSSetConstantBuffers(0, 1, m_ConstantBuffer.GetAddressOf());
		m_Context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

		m_Context->Draw(6, 0);
	}

	void GuiRenderer::Shutdown() {
		// ComPtr handles release automatically
	}
}