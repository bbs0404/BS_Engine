#pragma once

#include <queue>

#include "Component.h"

namespace BS_Engine
{
	static const int MAX_LIGHTS_PER_RENDERER = 4;

	enum LightType
	{
		Point,
		Directional
	};

	class Light : Component
	{
		struct Light_Compare;
	public:
		static Color ambient;

		LightType type = LightType::Directional;
		Color color = Color(1,1,1,1);
		float intensity = 1;
		int priority = 0;
		float range = 5;

		Vector3 LightDirection(Vector3 to);
		bool isReaching(Vector3 from);
	protected:
		Light(GameObject* gameObject);
	};

	struct LightCompare
	{
		bool operator() (const Light* lhs, const Light* rhs) const
		{
			return lhs->type > rhs->type ? lhs->type > rhs->type : lhs->priority > rhs->priority;
		}
	};
}