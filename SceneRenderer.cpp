#include "pch.h"
#include "SceneRenderer.h"

namespace BS_Engine
{
	SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources> deviceResources)
		:m_deviceResources(deviceResources)
	{
		CreateDeviceDependentResources();
		CreateWindowSizeDependentResources();
		LoadShader();
		activeScene = new Scene(deviceResources);
	}

	SceneRenderer::~SceneRenderer()
	{
	}

	
	void SceneRenderer::CreateDeviceDependentResources()
	{
	}

	void SceneRenderer::CreateWindowSizeDependentResources()
	{
	}

	void SceneRenderer::ReleaseDeviceDependentResources()
	{
	}

	void SceneRenderer::Update(DX::StepTimer const & timer)
	{
		activeScene->Update(timer);
	}

	void SceneRenderer::LoadShader()
	{
		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
		auto createVSTask = loadVSTask.then([&, this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_vertexShader
				)
			);
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					&fileData[0],
					fileData.size(),
					&m_inputLayout
				)
			);
		});



		auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");
		auto createPSTask = loadPSTask.then([&, this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_pixelShader
				)
			);
		});

		// 셰이더를 비동기적으로 로드합니다.

		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		DX::ThrowIfFailed(
			m_deviceResources->GetD3DDevice()->CreateBuffer(
				&constantBufferDesc,
				nullptr,
				&m_constantBuffer
			)
		);

		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0] = 0;
		samplerDesc.BorderColor[1] = 0;
		samplerDesc.BorderColor[2] = 0;
		samplerDesc.BorderColor[3] = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampleState;
		m_deviceResources->GetD3DDevice()->CreateSamplerState(&samplerDesc, &m_sampleState);

		// 큐브가 로드되면 개체가 렌더링할 수 있도록 준비됩니다.
		(createPSTask&&createVSTask).then([this]()
		{
			m_loadingComplete = true;
		});
	}

	void SceneRenderer::Render()
	{
		if (!m_loadingComplete)
			return;
		//
		Windows::Foundation::Size outputSize = m_deviceResources->GetOutputSize();
		float aspectRatio = outputSize.Width / outputSize.Height;
		//TODO : 매번 계산하지 않고 Window Size가 바뀔때만 계산하고 저장하여 사용할 수 있도록 하자.
		m_constantBufferData.projection =
			XMMatrixTranspose(Camera::main->PerspectiveMatrix(aspectRatio) * m_deviceResources->GetOrientationTransform3D());
		m_constantBufferData.view =
			XMMatrixTranspose(Camera::main->ViewMatrix());

		for each(GameObject* obj in activeScene->gameObjects)
		{
			Renderer* renderer = obj->GetComponent<Renderer>();
			if (renderer != nullptr)
			{
				MeshRenderer* meshRenderer = dynamic_cast<MeshRenderer*>(renderer);
				if (meshRenderer != nullptr)
				{
					std::shared_ptr<Material> material = renderer->material;
					m_constantBufferData.model =
						XMMatrixTranspose(renderer->gameObject.transform.WorldMatrix());
					meshRenderer->Render(m_deviceResources,
						m_inputLayout,
						m_vertexShader,
						m_pixelShader,
						m_constantBuffer,
						m_constantBufferData,
						m_sampleState
					);
				}
				else
				{
					std::shared_ptr<Material> material = renderer->material;
					m_constantBufferData.model =
						XMMatrixTranspose(renderer->gameObject.transform.WorldMatrix());
					renderer->Render(m_deviceResources,
						m_inputLayout,
						m_vertexShader,
						m_pixelShader,
						m_constantBuffer,
						m_constantBufferData
					);
				}
			}
		}
	}

	Scene* SceneRenderer::activeScene = nullptr;
}