#include "pch.h"
#include "Light.h"

namespace BS_Engine
{
	Vector3 Light::LightDirection(Vector3 to)
	{
		switch (type)
		{
		case Directional:
			return gameObject.transform.Front();
		case Point:
			return to - gameObject.transform.position;
		}
	}
	bool Light::isReaching(Vector3 from)
	{
		switch (type)
		{
		case Directional:
			return true;
		case Point:
			return Vector3::Distance(gameObject.transform.position, from) <= range;
		}
	}
	Light::Light(GameObject * gameObject)
		:Component(gameObject)
	{
	}
}