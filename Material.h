#pragma once
#include "SimpleMath.h"
#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"

using namespace DirectX::SimpleMath;
using namespace DirectX;

namespace BS_Engine
{
	struct Material
	{
	public:
		Color color;

		std::wstring vertexShaderName;
		std::wstring pixelShaderName;

		Microsoft::WRL::ComPtr<ID3D11Resource> diffuseTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTextureView;

		Material();
		Material(std::wstring vertexShader, std::wstring pixelShader);
		~Material();
		
		void LoadTex(const std::shared_ptr<DX::DeviceResources> deviceResources, const wchar_t* fileName);

		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayouts;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> m_sampleState;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
	};
}