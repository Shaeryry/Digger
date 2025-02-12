#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject* gameObject) :
	m_Destroyed{ false }
{
	SetParent(gameObject);
}

dae::Component::~Component()
{
}

void dae::Component::SetTransform(TransformComponent* transform)
{
	if (transform == nullptr) return; // Make sure that the transform exists;
	if (transform == this) return; // Make sure that the transform isn't the same component.
	m_Transform = transform;
}
