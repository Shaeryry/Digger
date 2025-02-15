#include <string>
#include <iostream>
#include <functional>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::GameObject()
{
	AddComponent<TransformComponent>(); // Add the base transform component to EVERY game object.
}

dae::GameObject::~GameObject()
{
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

	std::erase_if(m_Components, std::bind(&Component::IsDestroyed, std::placeholders::_1) );
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
