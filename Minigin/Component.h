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
		Component(GameObject* gameObject);
		virtual ~Component();
		Component(const Component& other) = delete;
		Component(Component&& other) = delete; 
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void Destroy() { m_Destroyed = true; };
		bool IsDestroyed() const { return m_Destroyed; };

		void SetParent(GameObject* gameObject) { m_Parent = gameObject; };
		GameObject* GetParent() const { return m_Parent; };

		void SetTransform(TransformComponent* transform); 
		TransformComponent* GetTransform() const { return m_Transform; }; 

		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const {}; 
	private:
		bool m_Destroyed;
		GameObject* m_Parent; // A pointer to the parented gameObject
		TransformComponent* m_Transform; // A pointer to the owner's transform for easy access. 
	};
}
