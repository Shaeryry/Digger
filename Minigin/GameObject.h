#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

namespace dae
{
	// class Texture2D;

	// todo: this should become final.
	class GameObject 
	{
	public:
		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		template<typename T> Component* AddComponent();
		template<typename T> T* GetComponent();

		void RemoveComponent(std::unique_ptr<Component> component);
	private:
		std::vector< std::unique_ptr<Component> > m_components{}; 

		// todo: mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_texture{};
	};
	 
	template<typename T>
	inline Component* GameObject::AddComponent()
	{
		std::unique_ptr<T> newComponent{ std::make_unique<T>() }; 
		m_components.emplace_back(std::move(newComponent));

		return m_components.back().get();
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (auto& component : m_components) {
			T* castedComponent = dynamic_cast<T*>( component.get() );

			if (castedComponent) {
				return castedComponent;
			}
		}

		return nullptr;
	}
}
