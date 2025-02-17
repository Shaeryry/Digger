#pragma once
#include "Component.h"

namespace dae {
	class GameObject;
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* gameObject,float orientation = 1.f);
		virtual void Update() override;
	private:
		float m_Radius{ 50 };
		float m_Orientation{ 1 };
	};
}
