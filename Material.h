#pragma once
#include "SimpleMath.h"
#include "..\Common\DeviceResources.h"
#include "..\Content\ShaderStructures.h"
#include "..\Common\StepTimer.h"
#include "..\Common\DirectXHelper.h"

using namespace DirectX::SimpleMath;

namespace BS_Engine
{
	struct Material
	{
	public:
		Color color;

		std::wstring vertexShaderName;
		std::wstring pixelShaderName;

		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;

		Material();
		Material(std::wstring vertexShader, std::wstring pixelShader);
		~Material();
	};
}