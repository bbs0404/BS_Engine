#pragma once
#include "Component.h"
namespace BS_Engine
{
	class Camera : public Component
	{
		friend class GameObject;
	private:
		int fov;
		Camera(GameObject*);
	public:
		static Camera* main;
		float nearZ, farZ;

		Matrix ViewMatrix();
		Matrix PerspectiveMatrix(float aspectRatio);

		void SetAsMain();

		void SetFOV(int value);
		int GetFOV();
	};
}
