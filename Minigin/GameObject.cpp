#include <string>
#include <iostream>
#include <functional>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

Rinigin::GameObject::GameObject()
	: m_IsActive{ true }
{
	m_Transform = AddComponent<TransformComponent>(); // Add the base transform component to EVERY game object.
}

Rinigin::GameObject::GameObject(GameObject* parent,bool keepPosition) : 
	GameObject()
{
	SetParent(parent,keepPosition);
}

Rinigin::GameObject::~GameObject()
{
}

// Parent

void Rinigin::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	// 1. Validation
	if (IsChild(parent) or parent == this or m_Parent == parent) return; // Check if the parent is valid !

	if (parent == nullptr) {
		SetPosition(GetWorldPosition());	// If the parent is not there set the local position to the world position
	}
	else {
		if (keepWorldPosition) {
			const glm::vec3 offsetPosition{ GetWorldPosition() - parent->GetWorldPosition() };
			SetPosition(offsetPosition);
		}

		m_Transform->MakePositionDirty();
	}

	if (m_Parent) m_Parent->RemoveChild(this); 
	m_Parent = parent;
	if (parent) parent->AddChild(this);
}


void Rinigin::GameObject::SetPosition(const glm::vec2& localPosition)
{
	SetPosition(localPosition.x, localPosition.y, 0.f);
}

void Rinigin::GameObject::SetPosition(const glm::vec3& localPosition)
{
	SetPosition(localPosition.x, localPosition.y, localPosition.z);
}

void Rinigin::GameObject::SetPosition(const float x, const float y, const float z)
{
	m_Transform->SetLocalPosition(x,y,z);
}

glm::vec3 Rinigin::GameObject::GetWorldPosition() const
{
	return m_Transform->GetTransformWorldPosition();
}

// Methods

void Rinigin::GameObject::FixedUpdate() 
{
	if (IsActive()) {
		for (auto& component : m_Components) {
			if (component->IsEnabled()) component->FixedUpdate();
		}
	}
}

void Rinigin::GameObject::Update()
{
	if (IsActive()) {
		for (auto& component : m_Components) {
			if (component->IsEnabled()) component->Update();
		}
	}
}

void Rinigin::GameObject::LateUpdate()
{
	if (IsActive()) {
		for (auto& component : m_Components) {
			if (component->IsEnabled()) component->LateUpdate();
		}
	}

	std::erase_if( m_Components, std::bind(&Component::IsDestroyed, std::placeholders::_1) );
} 

void Rinigin::GameObject::Render() const
{
	if (IsActive()) {
		for (auto& component : m_Components) {
			if (component->IsEnabled()) component->Render();
		}
	}
}

// Components

bool Rinigin::GameObject::HasComponent(Component* component)
{
	for (auto& comp : m_Components) {
		if (comp.get() == component) return true;
	}

	return false;
}

void Rinigin::GameObject::RemoveComponent(Component* component)
{
	if (component != nullptr and HasComponent(component) ) component->Destroy();
}

// Notifications

void Rinigin::GameObject::NotifyPositionChanged()
{
	std::for_each(m_Children.begin(), m_Children.end(), std::bind(&GameObject::OnParentPositionChanged, std::placeholders::_1)); // Call the on position changed event for all the children
}

void Rinigin::GameObject::OnParentPositionChanged()
{
	//SetPosition(m_Transform->GetTransformLocalPosition()); // Set the local position to the updated position
	m_Transform->MakePositionDirty();
}

// Children

bool Rinigin::GameObject::IsChild(GameObject* gameObject) const
{
	return std::find(m_Children.begin(), m_Children.end(), gameObject) != m_Children.end();
}

void Rinigin::GameObject::AddChild(GameObject* gameObject)
{
	if (!IsChild(gameObject)) {
		m_Children.emplace_back(gameObject);
	}
}
void Rinigin::GameObject::RemoveChild(GameObject* gameObject)
{
	if (IsChild(gameObject)) {
		std::erase(m_Children, gameObject);
	}
}
