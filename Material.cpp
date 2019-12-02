#include "pch.h"
#include "Material.h"

#include <WICTextureLoader.h>

namespace BS_Engine
{
	Material::Material()
		:vertexShaderName(L"SampleVertexShader.cso"), pixelShaderName(L"SamplePixelShader.cso")
	{
	}


	Material::Material(std::wstring vertexShader, std::wstring pixelShader)
		: vertexShaderName(vertexShader), pixelShaderName(pixelShader)
	{
	}

	Material::~Material()
	{
	}

	void Material::LoadTex(const std::shared_ptr<DX::DeviceResources> deviceResources, const wchar_t* fileName)
	{
		auto device = deviceResources->GetD3DDevice();
		HRESULT hr = CreateWICTextureFromFile(device, fileName, &diffuseTexture, diffuseTextureView.ReleaseAndGetAddressOf());
		if (FAILED(hr))
		{
			//ERROR!
		}
	}

}