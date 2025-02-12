#pragma once
#include <memory>
#include <vector>

namespace dae
{
	class GameObject;
	class TransformComponent;
	class Component
	{
	public:
		//Component() = default;
		Component(GameObject* parent);
		virtual ~Component();
		Component(const Component& other) = delete;
		Component(Component&& other) = delete; 
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void SetParent(GameObject* parent);
		GameObject* GetParent() const { return m_parent; };

		void SetTransform(TransformComponent* transform); 
		TransformComponent* GetTransform() const { return m_transform; }; 

		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const; 
	private:
		GameObject* m_parent; // A pointer to the parented gameobject
		TransformComponent* m_transform; // A pointer to the owner's transform for easy access. 
	};
}
