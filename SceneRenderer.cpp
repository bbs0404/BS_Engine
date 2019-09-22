#include "pch.h"
#include "SceneRenderer.h"

namespace BS_Engine
{
	SceneRenderer::SceneRenderer(const std::shared_ptr<DX::DeviceResources> deviceResources)
		:m_deviceResources(deviceResources)
	{
		CreateDeviceDependentResources();
		CreateWindowSizeDependentResources();
		LoadShaders();
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

	void SceneRenderer::LoadShaders()
	{
		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		auto loadVSTask = DX::ReadDataAsync(VertexShaderList[0]);
		auto createVSTask = loadVSTask.then([&, this](const std::vector<byte>& fileData)
		{
			m_vertexShaders.insert(
				std::pair<std::wstring, Microsoft::WRL::ComPtr<ID3D11VertexShader>>
				(VertexShaderList[0], Microsoft::WRL::ComPtr<ID3D11VertexShader>()));

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_vertexShaders[VertexShaderList[0]]
				)
			);

			m_inputLayouts.insert(
				std::pair<std::wstring, Microsoft::WRL::ComPtr<ID3D11InputLayout>>
				(VertexShaderList[0], Microsoft::WRL::ComPtr<ID3D11InputLayout>()));

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					&fileData[0],
					fileData.size(),
					&m_inputLayouts[VertexShaderList[0]]
				)
			);
		});



		auto loadPSTask = DX::ReadDataAsync(PixelShaderList[0]);
		auto createPSTask = loadPSTask.then([&, this](const std::vector<byte>& fileData)
		{
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&m_pixelShaders[PixelShaderList[0]]
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
				std::shared_ptr<Material> material = renderer->material;
				m_constantBufferData.model =
					XMMatrixTranspose(renderer->gameObject.transform.WorldMatrix());
				renderer->Render(m_deviceResources,
					m_inputLayouts[material->vertexShaderName],
					m_vertexShaders[material->vertexShaderName],
					m_pixelShaders[material->pixelShaderName],
					m_constantBuffer,
					m_constantBufferData
				);
			}
		}
	}

	Scene* SceneRenderer::activeScene = nullptr;

	std::vector<std::wstring> SceneRenderer::VertexShaderList = { L"SampleVertexShader.cso"};
	std::vector<std::wstring> SceneRenderer::PixelShaderList = { L"SamplePixelShader.cso" };
}