#include "pch.h"
#include "Camera.h"

namespace BS_Engine
{
	Camera* Camera::main = nullptr;

	Camera::Camera(GameObject * gameObject)
		:Component(gameObject), fov(60), nearZ(0.01f), farZ(1000)
	{
	}
	Matrix Camera::ViewMatrix()
	{
		Vector3 eye = gameObject.transform.position;
		Vector3 at = eye + gameObject.transform.Front();
		Vector3 up = Vector3::Up;
		return XMMatrixLookAtRH(eye, at, up);
	}
	Matrix Camera::PerspectiveMatrix(float aspectRatio)
	{
		float fovRadians = DirectX::XMConvertToRadians(fov);
		return Matrix::CreatePerspectiveFieldOfView(fovRadians, aspectRatio, nearZ, farZ);
	}
	
	inline void Camera::SetFOV(int value)
	{
		fov = std::min<int>(std::max<int>(30, value),89);
	}
	int Camera::GetFOV()
	{
		return fov;
	}
	void Camera::SetAsMain()
	{
		main = this;
	}
}