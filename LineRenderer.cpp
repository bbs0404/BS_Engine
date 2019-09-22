#include "pch.h"
#include "LineRenderer.h"

using namespace DirectX;

namespace BS_Engine
{
	LineRenderer::LineRenderer(GameObject* gameObject)
		:Renderer(gameObject)
	{
	}

	void LineRenderer::Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
		ModelViewProjectionConstantBuffer constantBufferData)
	{
		auto context = deviceResources->GetD3DDeviceContext();

		// 그래픽 장치로 보낼 수 있도록 상수 버퍼를 준비합니다.
		context->UpdateSubresource1(
			constantBuffer.Get(),
			0,
			NULL,
			&constantBufferData,
			0,
			0,
			0
		);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &positions[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(positions.size() * sizeof(VertexPositionColor), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
			)
		);

		// 각 꼭짓점은 VertexPositionColor 구조체의 한 인스턴스입니다.
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		context->IASetVertexBuffers(
			0,
			1,
			m_vertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		context->IASetInputLayout(inputLayout.Get());

		// 꼭짓점 셰이더를 연결합니다.
		context->VSSetShader(
			vertexShader.Get(),
			nullptr,
			0
		);

		// 상수 버퍼를 그래픽 장치에 보냅니다.
		context->VSSetConstantBuffers1(
			0,
			1,
			constantBuffer.GetAddressOf(),
			nullptr,
			nullptr
		);

		// 픽셀 셰이더를 연결합니다.
		context->PSSetShader(
			pixelShader.Get(),
			nullptr,
			0
		);

		// 개체를 그립니다.
		context->Draw(positions.size(), 0);
	}

	void LineRenderer::SetPositions(VertexPositionColor newPositions[])
	{
		int size = sizeof(newPositions) / sizeof(VertexPositionColor);
		positions = std::vector<VertexPositionColor>(newPositions, newPositions + size);
	}

	void LineRenderer::SetPositionAt(VertexPositionColor newPosition, int index)
	{
		if (index >= positions.size())
			SetSize(index);
		positions[index] = newPosition;
	}
}