#include "pch.h"
#include "MeshRenderer.h"

using namespace DirectX;

namespace BS_Engine
{
	MeshRenderer::MeshRenderer(GameObject* gameObject)
		:Renderer(gameObject)
	{
	}

	void MeshRenderer::Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
		Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
		ModelViewProjectionConstantBuffer constantBufferData)
	{

	}
	void MeshRenderer::Render(const std::shared_ptr<DX::DeviceResources>& deviceResources, Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout, Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader, Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer, ModelViewProjectionConstantBuffer constantBufferData, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState)
	{
		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &mesh->verticesData[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC vertexBufferDesc(mesh->verticesData.size() * sizeof(VertexData), D3D11_BIND_VERTEX_BUFFER);
		DX::ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexBufferData,
				&m_vertexBuffer
			)
		);

		m_indexCount = mesh->vIndices.size();

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = &mesh->vIndices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		CD3D11_BUFFER_DESC indexBufferDesc(m_indexCount * sizeof(unsigned int), D3D11_BIND_INDEX_BUFFER);
		DX::ThrowIfFailed(
			deviceResources->GetD3DDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexBufferData,
				&m_indexBuffer
			)
		);

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

		// �� �������� VertexPositionColor ����ü�� �� �ν��Ͻ��Դϴ�.
		UINT stride = sizeof(VertexData);
		UINT offset = 0;
		context->IASetVertexBuffers(
			0,
			1,
			m_vertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		context->IASetIndexBuffer(
			m_indexBuffer.Get(),
			DXGI_FORMAT_R32_UINT, // �� �ε����� �ϳ��� ��ȣ ���� 32��Ʈ ����(Int)�Դϴ�.
			0
		);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->IASetInputLayout(inputLayout.Get());

		context->PSSetShaderResources(0, 1, material->diffuseTextureView.GetAddressOf());

		// ������ ���̴��� �����մϴ�.
		context->VSSetShader(
			vertexShader.Get(),
			nullptr,
			0
		);

		// ��� ���۸� �׷��� ��ġ�� �����ϴ�.
		context->VSSetConstantBuffers(
			0,
			1,
			constantBuffer.GetAddressOf()
		);

		// �ȼ� ���̴��� �����մϴ�.
		context->PSSetShader(
			pixelShader.Get(),
			nullptr,
			0
		);

		context->PSSetSamplers(0, 1, sampleState.GetAddressOf());

		// ��ü�� �׸��ϴ�.
		context->DrawIndexed(m_indexCount, 0, 0);
	}
}