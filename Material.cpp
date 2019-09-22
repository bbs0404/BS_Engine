#include "pch.h"
#include "Material.h"

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
}