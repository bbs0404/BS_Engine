#include "GameObject.h"

namespace BS_Engine
{
	GameObject::GameObject()
		:transform(*this)
	{
	}

	GameObject::~GameObject()
	{
	}
}