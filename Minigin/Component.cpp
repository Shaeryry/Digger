#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject* parent) :
	m_Destroyed{ false }
{
	SetParent(parent);
}

dae::Component::~Component()
{
}

void dae::Component::Destroy()
{
	m_Destroyed = true;
}

void dae::Component::SetParent(GameObject* parent)
{
	m_Parent = parent;
}

void dae::Component::SetTransform(TransformComponent* transform)
{
	if (transform == nullptr) return; // Make sure that the transform exists;
	if (transform == this) return; // Make sure that the transform isn't the same component.
	m_Transform = transform; //std::shared_ptr<TransformComponent>(transform);
}

void dae::Component::FixedUpdate()
{
}

void dae::Component::Update()
{
}

void dae::Component::LateUpdate()
{
}

void dae::Component::Render() const
{
}
