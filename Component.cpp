#include "pch.h"
#include "Component.h"


namespace BS_Engine
{
	Component::Component(GameObject * gameObject)
		:gameObject(*gameObject)
	{
	}
}