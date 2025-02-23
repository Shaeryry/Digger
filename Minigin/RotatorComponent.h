#pragma once
#include "Component.h"

namespace dae {
	class GameObject;
	class RotatorComponent final : public Component
	{
	public:
		explicit RotatorComponent(GameObject* gameObject,float radius,float rotationTime = 1.f,float direction = 1.f);
		virtual void Update() override;
		void SetRadius(float radius) { m_Radius = radius; }
		void SetOrientation(float orientation) { m_Direction = orientation; };
		void SetRotationTime(float rotationTime) { m_RotationTime = rotationTime; };
	private:
		float m_Radius;
		float m_RotationTime;
		float m_Direction;
	};
}
