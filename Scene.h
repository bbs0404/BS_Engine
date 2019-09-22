#pragma once
#include "GameObject.h"
#include "Components.h"
#include <vector>
#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

namespace BS_Engine
{
	struct Scene
	{
		friend class BS_EngineMain;
		friend class SceneRenderer;
	private:
		std::vector<GameObject*> gameObjects;
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		void Start();
		void Update(DX::StepTimer const& timer);

	public:
		Scene(std::shared_ptr<DX::DeviceResources> deviceResources);
		~Scene();
	};
}

