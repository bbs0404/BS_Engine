#include "pch.h"
#include "GameObject.h"
#include "Camera.h"
#include "Scene.h"

namespace BS_Engine
{
	void GameObject::Start()
	{
		for each (Component* component in components)
		{
			component->Start();
		}
	}

	void GameObject::Update(DX::StepTimer const& timer)
	{
		for each (Component* component in components)
		{
			component->Update(timer);
		}
	}

	GameObject::GameObject()
		:transform(this)
	{
	}

	GameObject::~GameObject()
	{
		for each (Component* component in components)
		{
			delete component;
		}
	}
}