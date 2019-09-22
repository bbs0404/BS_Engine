#include "pch.h"
#include "Transform.h"

namespace BS_Engine
{
	Transform::Transform(GameObject* gameObject)
		:gameObject(gameObject), position(), rotation(), scale(1, 1, 1)
	{
	}


	Transform::~Transform()
	{
	}

	Quaternion Transform::Rotation()
	{
		float x = DirectX::XMConvertToRadians(rotation.x);
		float y = DirectX::XMConvertToRadians(rotation.y);
		float z = DirectX::XMConvertToRadians(rotation.z);
		return Quaternion::CreateFromAxisAngle(Vector3(1, 0, 0), x) *
			Quaternion::CreateFromAxisAngle(Vector3(0, 1, 0), y) *
			Quaternion::CreateFromAxisAngle(Vector3(0, 0, 1), z);
	}
}