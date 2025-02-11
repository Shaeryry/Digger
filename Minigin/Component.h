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
		Component() = default;
		virtual ~Component();
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		void SetTransform(TransformComponent* transform);
		virtual void Update();
		virtual void Render() const; 
	protected:
		//std::shared_ptr<TransformComponent> m_transform; // A pointer to the owner's transform for easy access.
		TransformComponent* m_transform{};
	};
}
