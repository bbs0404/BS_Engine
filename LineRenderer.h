#pragma once
#include "Renderer.h"

namespace BS_Engine
{
	class LineRenderer : public Renderer
	{
		friend class GameObject;
	private:
		LineRenderer(GameObject* gameObject);
	public:

		std::vector<VertexPositionColor> positions;

		void Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
			Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
			ModelViewProjectionConstantBuffer constantBufferData) override;

		inline void SetPositions(std::vector<VertexPositionColor> newPositions) { positions = newPositions; }
		void SetPositions(VertexPositionColor newPositions[]);

		inline void AddPosition(VertexPositionColor newPosition) { positions.push_back(newPosition); }
		void SetPositionAt(VertexPositionColor newPosition, int index);

		inline void SetSize(int newSize) { positions.resize(newSize); }
	};
}