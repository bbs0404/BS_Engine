#pragma once
#include "SimpleMath.h"
#include "GameObject.h"

using namespace DirectX::SimpleMath;

namespace BS_Engine
{
	class Component
	{
		friend class GameObject;
	private:
		virtual void Start() {}
		virtual void Update(DX::StepTimer const& timer) {}
	protected:
		Component(GameObject* gameObject);
	public:
		GameObject& gameObject;
	};
}