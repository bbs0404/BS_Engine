#pragma once
#include "Component.h"
#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"

#include "Material.h"

namespace BS_Engine
{
	class Renderer : public Component
	{
		friend class GameObject;
	protected:
		Renderer(GameObject* gameObject)
			:Component(gameObject)
		{}

		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_indexBuffer;

		//ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		bool	m_loadingComplete;
	public:
		std::shared_ptr<Material> material;

		virtual void Render(const std::shared_ptr<DX::DeviceResources>& deviceResources,
			Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout,
			Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader,
			Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader,
			Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer,
			ModelViewProjectionConstantBuffer constantBufferData) = 0;
	};

	enum RenderType
	{
		COLOR,
		COLOR_SHADING,
		TEXTURE,
		TEXTURE_SHADING,
	};
}