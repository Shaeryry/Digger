#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

namespace dae
{
	class GameObject final
	{
	public:

		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		TransformComponent* GetTransform() const { return m_Transform; };

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;

		template<typename T,typename... Arguments> T* AddComponent(Arguments&&... args);
		template<typename T> void RemoveComponent();
		template<typename T> T* GetComponent();

		void RemoveComponent(Component* component);
	private:
		std::vector< std::unique_ptr<Component> > m_Components{}; 
		TransformComponent* m_Transform;
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
