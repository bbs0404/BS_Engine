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

		// �׷��� ��ġ�� ���� �� �ֵ��� ��� ���۸� �غ��մϴ�.
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

		// �� �������� VertexPositionColor ����ü�� �� �ν��Ͻ��Դϴ�.
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

		// ������ ���̴��� �����մϴ�.
		context->VSSetShader(
			vertexShader.Get(),
			nullptr,
			0
		);

		// ��� ���۸� �׷��� ��ġ�� �����ϴ�.
		context->VSSetConstantBuffers1(
			0,
			1,
			constantBuffer.GetAddressOf(),
			nullptr,
			nullptr
		);

		// �ȼ� ���̴��� �����մϴ�.
		context->PSSetShader(
			pixelShader.Get(),
			nullptr,
			0
		);

		// ��ü�� �׸��ϴ�.
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