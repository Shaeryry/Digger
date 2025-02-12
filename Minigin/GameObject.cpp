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
	for (auto& component : m_components) {
		component->FixedUpdate();
	}
}

void dae::GameObject::Update()
{
	for (auto& component : m_components) {
		component->Update();
	}
}

void dae::GameObject::LateUpdate()
{
	//std::vector< std::unique_ptr<Component> > destroyedComponents{};

	for (auto& component : m_components) {
		component->LateUpdate();
	}

	m_components.erase( 
		std::remove_if(m_components.begin(), m_components.end(),std::bind(&Component::IsDestroyed,std::placeholders::_1)),
		m_components.end() 
	);
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components) {
		component->Render();
	}
}


void dae::GameObject::RemoveComponent(Component* component)
{
	if (component != nullptr) component->Destroy();
}
