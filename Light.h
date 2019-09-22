#pragma once

#include "Component.h"

namespace BS_Engine
{
	class Light : Component
	{
	public:
		virtual ~Light();

		virtual Vector3 LightDirection(Vector3 to) = 0;
	};

}