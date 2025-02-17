#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

namespace dae
{
	class GameObject final
	{
	public:
		explicit GameObject();
		explicit GameObject(GameObject* parent);

		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void SetParent(GameObject* parent, bool keepWorldPosition = false);
		bool IsChild(GameObject* gameObject) const;

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		template<typename T,typename... Arguments> T* AddComponent(Arguments&&... args);
		template<typename T> void RemoveComponent();
		template<typename T> T* GetComponent();

		void RemoveComponent(Component* component);

		TransformComponent* GetTransform() const { return m_Transform; };
		glm::vec3 GetWorldPosition();
		void UpdateWorldPosition();
		 
		void SetPosition(const glm::vec3& localPosition);
		void MarkPositionForUpdate() { m_PositionChanged = true; };
	private:
		void AddChild(GameObject* gameObject);
		void RemoveChild(GameObject* gameObject);

		GameObject* m_Parent;
		TransformComponent* m_Transform;
		bool m_PositionChanged;

		std::vector< std::unique_ptr<Component> > m_Components{}; 
		std::vector< GameObject* > m_Children{};
	};
	 
	template<typename T, typename... Arguments>	
	inline T* GameObject::AddComponent(Arguments&&... args)
	{
		std::unique_ptr<T> newComponent{ std::make_unique<T>( this,std::forward<Arguments>(args)... ) };
		m_Components.emplace_back( std::move(newComponent) );

		return dynamic_cast<T*>( m_Components.back().get() );
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		GameObject::RemoveComponent( GetComponent<T>() );
	}

	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (auto& component : m_Components) {
			T* castedComponent = dynamic_cast<T*>( component.get() );

			if (castedComponent) {
				return castedComponent;
			}
		}

		return nullptr;
	}
}
