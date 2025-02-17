#include <string>
#include <iostream>
#include <functional>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject() :
	m_PositionChanged{ true }
{
	m_Transform = AddComponent<TransformComponent>(); // Add the base transform component to EVERY game object.
}

dae::GameObject::GameObject(GameObject* parent) : 
	GameObject()
{
	SetParent(parent,true);
}

dae::GameObject::~GameObject()
{
}

void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// 1. Validation
	if (IsChild(parent) or parent == this or m_Parent == parent) return; // Check if the parent is valid !

	if (parent == nullptr) {
		SetPosition( GetWorldPosition() );	// If the parent is not there set the local position to the world position
	}
	else {
		if (keepWorldPosition) {
			const glm::vec3 offsetPosition{ GetWorldPosition() - parent->GetWorldPosition() };

			SetPosition(offsetPosition);
		}
		MarkPositionForUpdate();
	}

	if (m_Parent) m_Parent->RemoveChild(this); 
	m_Parent = parent;
	if (parent) parent->AddChild(this);
}

bool dae::GameObject::IsChild(GameObject* gameObject) const
{
	return std::find(m_Children.begin(), m_Children.end(), gameObject) != m_Children.end();
}

void dae::GameObject::FixedUpdate() 
{
	for (auto& component : m_Components) {
		component->FixedUpdate();
	}
}

void dae::GameObject::Update()
{
	for (auto& component : m_Components) {
		component->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	for (auto& component : m_Components) {
		component->LateUpdate();
	}

	std::erase_if( m_Components, std::bind(&Component::IsDestroyed, std::placeholders::_1) );
} 

void dae::GameObject::Render() const
{
	for (auto& component : m_Components) {
		component->Render();
	}
}


void dae::GameObject::RemoveComponent(Component* component)
{
	if (component != nullptr) component->Destroy();
}

glm::vec3 dae::GameObject::GetWorldPosition()
{
	UpdateWorldPosition();
	return m_Transform->GetTransformWorldPosition();
}

void dae::GameObject::UpdateWorldPosition()
{
	if (m_PositionChanged) {
		if (m_Parent) {
			m_Transform->SetWorldPosition(m_Parent->GetWorldPosition() + m_Transform->GetTransformLocalPosition());
		}
		else {
			m_Transform->SetWorldPosition(m_Transform->GetTransformLocalPosition());
		}

		// Mark all the children for update !
		//std::for_each(m_Children.begin(), m_Children.end(), std::bind(&GameObject::MarkPositionForUpdate, std::placeholders::_1));

		m_PositionChanged = false;
	}
}

void dae::GameObject::SetPosition(const glm::vec3& localPosition)
{
	m_Transform->SetLocalPosition(localPosition);
	MarkPositionForUpdate();
}

void dae::GameObject::AddChild(GameObject* gameObject)
{
	if (!IsChild(gameObject)) {
		m_Children.emplace_back(gameObject);
	}
}

void dae::GameObject::RemoveChild(GameObject* gameObject)
{
	std::erase(m_Children, gameObject);
	//m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), gameObject), m_Children.end());
}
