#include "Transform.h"

namespace BS_Engine
{
	Transform::Transform(GameObject& gameObject)
		:gameObject(gameObject), position(), rotation(), scale(1, 1, 1)
	{
	}


	Transform::~Transform()
	{
	}
}