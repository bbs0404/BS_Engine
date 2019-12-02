#pragma once

#include <vector>

#include "Common\StepTimer.h"
#include "Transform.h"

namespace BS_Engine
{
	class Component;

	class GameObject
	{
		friend class Scene;
	private:
		void Start();
		void Update(DX::StepTimer const& timer);
	public:
		GameObject();
		~GameObject();

		Transform transform;

		std::vector<Component*> components;

		template<class T>
		T* AddComponent()
		{
			T* component = new T(this);
			components.push_back(component);
			return component;
		}

		template<class T>
		T* GetComponent()
		{
			for each (Component* component in components)
			{
				T* casted = dynamic_cast<T*>(component);
				if (casted)
				{
					return casted;
				}
			}
			return nullptr;
		}
	};
}