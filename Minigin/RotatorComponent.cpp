#include "RotatorComponent.h"
#include "TransformComponent.h"
#include "GameObject.h"
#include "Timer.h"

#define _USE_MATH_DEFINES
#include <math.h>

dae::RotatorComponent::RotatorComponent(GameObject* gameObject, float orientation) :
	Component(gameObject),
	m_Orientation{ orientation }
{
}

void dae::RotatorComponent::Update()
{
	float angle{ 2 * static_cast<float>(M_PI) * Timer::GetInstance().GetTotalElapsedSec() };
	float xRotation{ m_Radius * cosf(angle*m_Orientation) };
	float yRotation{ m_Radius * sinf(angle*m_Orientation) };

	GetOwner()->SetPosition( glm::vec3( xRotation,yRotation,0 ) );
}
