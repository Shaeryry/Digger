#include "Component.h"
#include "GameObject.h"

dae::Component::Component(GameObject* gameObject) :
	m_Destroyed{ false },
	m_Owner{ gameObject }
{
}

dae::Component::~Component()
{
}
