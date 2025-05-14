#include "PhysicsManager.h"
#include "ColliderComponent.h"

#include "Renderer.h"
#include <glm.hpp>
#include <SDL.h>

#include <iostream>

void Rinigin::Physics::Render() const
{
	auto* renderer{ Renderer::GetInstance().GetSDLRenderer() };

	for (auto* collider : m_Colliders) {
		const glm::vec3 position{ collider->GetPosition() };
		const glm::vec3 bounds{ collider->Bounds() };

		SDL_Rect rect{ 
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			static_cast<int>(bounds.x),
			static_cast<int>(bounds.y)
		};

		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Rinigin::Physics::FixedUpdate()
{
	if (m_Colliders.empty()) return;
	for (auto* collider : m_Colliders) {
		
		for (auto* other : m_Colliders) {
			if (other == collider) continue;
			const bool wasColliding = collider->IsColliding(other);
			const bool currentlyColliding = IsColliding(collider, other);

			if (wasColliding) {
				// The collider remembers this other collide
				if (!currentlyColliding) { // The collider lost contact with the other collider !
					collider->RemoveCollidingCollider(other);
				}
			}
			else { // The collider wasn't colliding with this collider before !
				if (currentlyColliding) { // The collider is now colliding however 
					collider->AddCollidingCollider(other);
				}
			}

 		}
	}
}

void Rinigin::Physics::AddCollider(ColliderComponent* collider)
{
	m_Colliders.emplace_back(collider);
}

void Rinigin::Physics::RemoveCollider(ColliderComponent* collider)
{
	if (m_Colliders.empty()) return;
	m_Colliders.erase(std::remove(m_Colliders.begin(), m_Colliders.end(), collider), m_Colliders.end());
}

bool Rinigin::Physics::IsColliding(ColliderComponent* collider_A, ColliderComponent* collider_B)
{
	const glm::vec3 positionA{ collider_A->GetPosition() };
	const glm::vec3 boundsA{ collider_A->Bounds() };

	const glm::vec3 positionB{ collider_B->GetPosition() };
	const glm::vec3 boundsB{ collider_B->Bounds() };

	return
		positionA.x < positionB.x + boundsB.x &&
		positionA.x + boundsA.x > boundsB.x &&
		positionA.y < positionB.y + boundsB.y &&
		positionA.y + boundsA.y > boundsB.y;
}

