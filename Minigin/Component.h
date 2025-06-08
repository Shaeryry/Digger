#pragma once
#include <memory>
#include <vector>

namespace Rinigin
{
	class GameObject;
	class Component
	{
	public:
		virtual ~Component();
		Component(const Component& other) = delete;
		Component(Component&& other) = delete; 
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void Destroy() { m_Destroyed = true; };
		void SetEnabled(bool state) { m_Enabled = state; };
		bool IsDestroyed() const { return m_Destroyed; };
		bool IsEnabled() const { return m_Enabled; }

		GameObject* GetOwner() const { return m_Owner; };

		virtual void FixedUpdate() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void Render() const {}; 
	protected:
		explicit Component(GameObject* gameObject);
	private:
		GameObject* m_Owner; // A pointer to the parented gameObject
		bool m_Destroyed;
		bool m_Enabled;
	};
}
