#include "ColliderComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "PhysicsManager.h"
#include <iostream>

Rinigin::ColliderComponent::ColliderComponent(GameObject* gameObject, const glm::vec3& size, const glm::vec3& offset, bool isTrigger) :
	Component(gameObject),

	m_ColliderEnterEvent( std::make_unique<Event>( Rinigin::CollisionEventArguments("CollisionEnter",this) ) ),
	m_ColliderExitEvent( std::make_unique<Event>(Rinigin::CollisionEventArguments("CollisionExit", this) ) ),
	m_IsTrigger(isTrigger),

	m_Bounds(size),
	m_Offset(offset)
{
	Physics::GetInstance().AddCollider(this);
}

Rinigin::ColliderComponent::~ColliderComponent()
{
	Physics::GetInstance().RemoveCollider(this);
}

void Rinigin::ColliderComponent::FixedUpdate()
{
}

glm::vec3 Rinigin::ColliderComponent::GetCenter() const {
	return GetPosition() + (m_Bounds * 0.5f);
}

glm::vec3 Rinigin::ColliderComponent::GetHalfExtents() const {
	return m_Bounds * 0.5f;
}

glm::vec3 Rinigin::ColliderComponent::GetPosition() const
{
	glm::vec3 origin{ GetOwner()->GetWorldPosition() };
	return (origin + m_Offset);
}

void Rinigin::ColliderComponent::AddCollidingCollider(ColliderComponent* other)
{
	if ( IsTouching(other) ) return;
	m_CollidingColliders.emplace_back(other);
	std::cout << "did collision" << std::endl;
	m_ColliderEnterEvent->NotifyObservers();
}

void Rinigin::ColliderComponent::RemoveCollidingCollider(ColliderComponent* other)
{	
	if (m_CollidingColliders.empty()) return;
	m_CollidingColliders.erase(std::remove(m_CollidingColliders.begin(), m_CollidingColliders.end(), other), m_CollidingColliders.end());
	std::cout << "lost collision" << std::endl;

	m_ColliderExitEvent->NotifyObservers();
}

bool Rinigin::ColliderComponent::IsTouching(ColliderComponent* other)
{
	auto foundIt = std::find(m_CollidingColliders.begin(), m_CollidingColliders.end(), other);
	return (foundIt != m_CollidingColliders.end());
}

void Rinigin::ColliderComponent::SetLayer(const char* layerName)
{
	SetLayer(Rinigin::Helpers::sdbm_hash(layerName));
}

void Rinigin::ColliderComponent::AddExcludedLayer(const char* layerName)
{
	AddExcludedLayer(Rinigin::Helpers::sdbm_hash(layerName));
}

void Rinigin::ColliderComponent::AddExcludedLayer(unsigned int layerId)
{
	m_ExcludedLayers.emplace_back(layerId);
}

void Rinigin::ColliderComponent::RemoveExcludedLayer(unsigned int layerId) 
{
	std::erase_if(m_ExcludedLayers, [&layerId](const auto& id) {
		return (id == layerId);
		}
	);
}

bool Rinigin::ColliderComponent::IsLayerExcluded(unsigned int layerId)
{
	auto foundIt = std::find(m_ExcludedLayers.begin(), m_ExcludedLayers.end(), layerId);
	return (foundIt != m_ExcludedLayers.end());
}
