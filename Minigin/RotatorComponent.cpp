#include "RotatorComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Timer.h"

#define _USE_MATH_DEFINES
#include <math.h>

dae::RotatorComponent::RotatorComponent(GameObject* gameObject,float radius,float rotationTime, float direction) :
	Component(gameObject),
	m_Radius{ radius },
	m_RotationTime{ rotationTime },
	m_Direction{ direction }
{
}

void dae::RotatorComponent::Update()
{
	float angle{ 2 * static_cast<float>(M_PI) * (Timer::GetInstance().GetTotalElapsedSec() / m_RotationTime) };
	float xRotation{ m_Radius * cosf(angle*m_Direction) };
	float yRotation{ m_Radius * sinf(angle*m_Direction) };

	GetOwner()->SetPosition( glm::vec3( xRotation,yRotation,0 ) );
}
