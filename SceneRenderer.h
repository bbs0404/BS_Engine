#pragma once
#include "Scene.h"
#include <map>
#include <vector>
namespace BS_Engine
{
	class SceneRenderer
	{
		friend class BS_EngineMain;
	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11InputLayout>> m_inputLayouts;
		std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11VertexShader>> m_vertexShaders;
		std::map<std::wstring, Microsoft::WRL::ComPtr<ID3D11PixelShader>> m_pixelShaders;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

		SceneRenderer(std::shared_ptr<DX::DeviceResources>);
		inline void Start() { /*activeScene->Start();*/ }
		void Update(DX::StepTimer const& timer);

		void LoadShaders();

	public:
		~SceneRenderer();

		static Scene* activeScene;

		void CreateDeviceDependentResources();
		void CreateWindowSizeDependentResources();
		void ReleaseDeviceDependentResources();

		void Render();

	private:

		ModelViewProjectionConstantBuffer	m_constantBufferData;
		uint32	m_indexCount;

		// 렌더링 루프에 사용되는 변수입니다.
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;

		static std::vector<std::wstring> VertexShaderList;
		static std::vector<std::wstring> PixelShaderList;
	};

}