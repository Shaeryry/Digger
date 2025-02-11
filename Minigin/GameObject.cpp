#include <string>
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

void dae::GameObject::Update()
{
	for (auto& component : m_components) {
		component->Update();
	}
}

void dae::GameObject::LateUpdate()
{
}

void dae::GameObject::Render() const
{
	for (auto& component : m_components) {
		component->Render();
	}
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
}



void dae::GameObject::RemoveComponent(std::unique_ptr<Component> component)
{
	m_components.erase(std::remove(m_components.begin(), m_components.end(), component), m_components.end());
}
