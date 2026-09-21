#pragma once

#include "nospch.h"

namespace nOs {
	class GuiRenderer {
	public:
		bool Init(ID3D11Device* device, ID3D11DeviceContext* context, UINT screenWidth, UINT screenHeight);
		void Shutdown();

		void DrawRect(float x, float y, float width, float height, float red, float green, float blue, float alpha);

	private:
		ID3D11Device* m_Device = nullptr;
		ID3D11DeviceContext* m_Context = nullptr;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_ConstantBuffer;

		DirectX::XMMATRIX m_ProjMatrix;

		struct Vertex {
			float x, y;
			float r, g, b, a;
		};
	};
}