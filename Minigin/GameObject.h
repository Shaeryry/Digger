#pragma once
#include <memory>
#include "TransformComponent.h"
#include "Component.h"

namespace dae
{
	// class Texture2D;

	// todo: this should become final.
	class GameObject final
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

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		template<typename T,typename... Arguments> T* AddComponent(Arguments&&... args);
		template<typename T> void RemoveComponent();
		template<typename T> T* GetComponent();

		void RemoveComponent(Component* component);
	private:
		std::vector< std::unique_ptr<Component> > m_Components{}; 

		// todo: mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_texture{};
	};
	 
	template<typename T, typename... Arguments>	
	inline T* GameObject::AddComponent(Arguments&&... args)
	{
		std::unique_ptr<T> newComponent{ std::make_unique<T>( this,std::forward<Arguments>(args)... ) };
		newComponent->SetTransform(GetComponent<TransformComponent>());

		m_Components.emplace_back(std::move(newComponent));

		return dynamic_cast<T*>( m_Components.back().get() );
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		T* objectToDelete{ GetComponent<T>() };
		RemoveComponent(objectToDelete);
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
