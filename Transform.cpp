#include "pch.h"
#include "Transform.h"

namespace BS_Engine
{
	Transform::Transform(GameObject* gameObject)
		:gameObject(*gameObject), position(), rotation(), scale(1, 1, 1)
	{
	}


	Transform::~Transform()
	{
	}

	void Transform::Rotate(const Vector3& eulerAngles)
	{
		float x = DirectX::XMConvertToRadians(eulerAngles.x);
		float y = DirectX::XMConvertToRadians(eulerAngles.y);
		float z = DirectX::XMConvertToRadians(eulerAngles.z);
		Rotate(Quaternion::CreateFromYawPitchRoll(y, x, z));
	}
	void Transform::Rotate(const Quaternion& rotation)
	{
		this->rotation *= rotation;
	}

	Vector3 Transform::GetEulerAngles()
	{
		Quaternion &q = rotation;
		Vector3 euler;
		euler.y = -DirectX::XMConvertToDegrees(atan2f(2.0f * (q.x * q.z + q.w * q.y), -q.x * q.x - q.y * q.y + q.z * q.z + q.w * q.w));
		euler.x = -DirectX::XMConvertToDegrees(asinf(2.0f * (q.w * q.x - q.y * q.z)));
		euler.z = DirectX::XMConvertToDegrees(atan2f(2.0f * (q.x * q.y + q.w * q.z), -q.x * q.x + q.y * q.y - q.z * q.z + q.w * q.w));
		return euler;
	}

	void Transform::LookAt(const Transform & transform)
	{
		LookAt(transform.position);
	}
	void Transform::LookAt(const Vector3& position)
	{
		Matrix mat = Matrix::CreateLookAt(this->position, position, Vector3::Up);
		mat = mat.Transpose();
		Rotate(Quaternion::CreateFromRotationMatrix(mat));
		Vector3 euler = GetEulerAngles();
		//rotation = Quaternion::CreateFromYawPitchRoll(euler.y, euler.x, euler.z);
	}
}