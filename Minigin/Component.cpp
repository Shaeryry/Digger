#include "Component.h"
#include "GameObject.h"

Rinigin::Component::Component(GameObject* gameObject) :
	m_Destroyed(false),
	m_Enabled(true),
	m_Owner(gameObject)
{
}

Rinigin::Component::~Component()
{
}
