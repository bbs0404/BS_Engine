#pragma once
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

namespace BS_Engine
{
	class Transform
	{
	private:
		friend class GameObject;					//Only GameObject can access constructor of Transform

		Transform(GameObject*);		
		~Transform();

		Transform(const Transform&) = delete;		//Cannot use Copy Construct
		void* operator new(size_t) = delete;		//Cannot use New Operator
	public:
		Vector3 position;							//Position of GameObject
		Vector3 rotation;							//Euler Angles of GameObject
		Vector3 scale;								//Scale of GameObject
		
		GameObject& gameObject;						//GameObject where this Transform attached to

		#pragma region Matrix
		inline Matrix TranslateMatrix() { return Matrix::CreateTranslation(position); }
		inline Matrix RotateMatrix() { return Matrix::CreateFromQuaternion(Rotation()); /*CreateFromYawPitchRoll(rotation.y, rotation.x, rotation.z);*/ }
		inline Matrix ScaleMatrix() { return Matrix::CreateScale(scale); }
		inline Matrix WorldMatrix() { return ScaleMatrix() * RotateMatrix() * TranslateMatrix(); }
		#pragma endregion

		#pragma region Directions
		inline Vector3 Front() { return Vector3::Transform(Vector3::Forward, RotateMatrix()); }
		inline Vector3 Back() { return -Front(); }
		inline Vector3 Right() { return Vector3::Transform(Vector3::Right, RotateMatrix()); }
		inline Vector3 Left() { return -Right(); }
		inline Vector3 Up() { return Vector3::Transform(Vector3::Up, RotateMatrix()); }
		inline Vector3 Down() { return -Up(); }
		#pragma endregion

		Quaternion Rotation();

		template<class T>
		T* AddComponent()
		{
			return gameObject.AddComponent<T>();
		}
		template<class T>
		T* GetComponent()
		{
			return gameObject.GetComponent<T>();
		}
	};
}