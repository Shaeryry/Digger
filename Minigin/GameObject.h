#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

namespace Rinigin
{
	class GameObject final
	{
	public:
		explicit GameObject();
		explicit GameObject(GameObject* parent, bool keepPosition = false);

		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		GameObject* GetParent() const { return m_Parent; };
		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		void SetActive(bool active) { m_IsActive = active; }
		bool IsActive() const { return m_IsActive; };

		// Transformations

		TransformComponent* GetTransform() const { return m_Transform; };

		void SetPosition(const glm::vec2& localPosition);
		void SetPosition(const glm::vec3& localPosition);
		void SetPosition(const float x, const float y, const float z);
		glm::vec3 GetWorldPosition() const;

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		// Components

		template<typename ComponentType,typename... Arguments> ComponentType* AddComponent(Arguments&&... args);
		template<typename ComponentType> void RemoveComponent();
		template<typename ComponentType> ComponentType* GetComponent();
		
		bool HasComponent(Component* component);
		void RemoveComponent(Component* component);

		// Notification Events

		void NotifyPositionChanged();

		// Listener Events

		void OnParentPositionChanged();

	private:
		bool IsChild(GameObject* gameObject) const;
		void AddChild(GameObject* gameObject);
		void RemoveChild(GameObject* gameObject);

		GameObject* m_Parent;
		TransformComponent* m_Transform;

		std::vector< std::unique_ptr<Component> > m_Components{}; 
		std::vector< GameObject* > m_Children{};
		bool m_IsActive;
	};
	 
	template<typename ComponentType, typename... Arguments>	
	inline ComponentType* GameObject::AddComponent(Arguments&&... args)
	{
		std::unique_ptr<ComponentType> newComponent{ std::make_unique<ComponentType>( this,std::forward<Arguments>(args)... ) };
		m_Components.emplace_back( std::move(newComponent) );

		return dynamic_cast<ComponentType*>( m_Components.back().get() );
	}

	template<typename ComponentType>
	inline void GameObject::RemoveComponent()
	{
		GameObject::RemoveComponent( GetComponent<ComponentType>() );
	}

	template<typename ComponentType>
	inline ComponentType* GameObject::GetComponent()
	{
		for (auto& component : m_Components) {
			ComponentType* castedComponent = dynamic_cast<ComponentType*>( component.get() );

			if (castedComponent) {
				return castedComponent;
			}
		}

		return nullptr;
	}
}
