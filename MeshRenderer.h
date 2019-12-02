#pragma once
#include "Renderer.h"
#include "Mesh.h"
#include "CommonStates.h"

namespace BS_Engine
{
	class MeshRenderer : public Renderer
	{
		friend class GameObject;
	private:
		MeshRenderer(GameObject* gameObject);
	public:
		std::shared_ptr<Mesh> mesh;

		void Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
			Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
			ModelViewProjectionConstantBuffer constantBufferData) override;

		void Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
			Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
			ModelViewProjectionConstantBuffer constantBufferData,
			Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState);
	};
}