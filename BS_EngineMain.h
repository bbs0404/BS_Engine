#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

#include "Scene.h"
#include "SceneRenderer.h"

// 화면의 Direct2D 및 3D 콘텐츠를 렌더링합니다.
namespace BS_Engine
{
	class BS_EngineMain : public DX::IDeviceNotify
	{
	public:
		BS_EngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~BS_EngineMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// 장치 리소스에 대한 캐시된 포인터입니다.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		//std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SceneRenderer> m_sceneRenderer;

		// TODO: 사용자 콘텐츠 렌더러로 대체합니다.
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// 렌더링 루프 타이머입니다.
		DX::StepTimer m_timer;
	};
}