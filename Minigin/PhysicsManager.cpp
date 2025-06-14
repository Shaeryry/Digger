#include "PhysicsManager.h"
#include "GameObject.h"

#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "ICollisionMask.h"

#include "Renderer.h"
#include <glm.hpp>
#include <SDL.h>

#include <iostream>

void Rinigin::Physics::Render() const
{
	if (not DEBUG_RENDER) return;
	auto* renderer{ Renderer::GetInstance().GetSDLRenderer() };

	for (auto* collider : m_Colliders) {
		if (!collider->GetOwner()->IsActive()) continue;
		const glm::vec3 position{ collider->GetPosition() };
		const glm::vec3 bounds{ collider->Bounds() };

		const SDL_Rect rect{ 
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			static_cast<int>(bounds.x),
			static_cast<int>(bounds.y)
		};

		if (collider->GetCollisions() > 0) {
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // red
		}
		else {
			if (collider->IsTrigger()) {
				SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // green
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // green
			}
		};

		SDL_RenderDrawRect(renderer, &rect);
	}
}

void Rinigin::Physics::FixedUpdate()
{
	if (m_Colliders.empty()) return;

	SolveCollisions(); // Solve rigid bodies
	DetectCollisions(); // Trigger enter/Exit events on colliders
}

void Rinigin::Physics::AddCollider(ColliderComponent* collider)
{
	m_Colliders.emplace_back(collider);
}

void Rinigin::Physics::RemoveCollider(ColliderComponent* collider)
{
	if (m_Colliders.empty()) return;

	// Remove it from the others
	for (ColliderComponent* other : m_Colliders) {
		if (other == collider) continue;
		other->RemoveCollidingCollider(collider);
	}

	std::erase(m_Colliders, collider);
}

void Rinigin::Physics::AddRigidbody(RigidbodyComponent* rigidbody)
{
	m_Rigidbodies.emplace_back(rigidbody);

}

void Rinigin::Physics::RemoveRigidbody(RigidbodyComponent* rigidbody)
{
	if (m_Rigidbodies.empty()) return;

	std::erase(m_Rigidbodies, rigidbody);
}


void Rinigin::Physics::AddCollisionMask(ICollisionMask* mask)
{
	m_CollisionMasks.emplace_back(mask);
}

void Rinigin::Physics::RemoveCollisionMask(ICollisionMask* mask)
{
	if (m_CollisionMasks.empty()) return;

	std::erase(m_CollisionMasks, mask);
}


bool Rinigin::Physics::AreCollidersOverlapping(ColliderComponent* collider_A, ColliderComponent* collider_B)
{
	const glm::vec3 positionA{ collider_A->GetPosition() };
	const glm::vec3 boundsA{ collider_A->Bounds() };

	const glm::vec3 positionB{ collider_B->GetPosition() };
	const glm::vec3 boundsB{ collider_B->Bounds() };

	return
		(positionA.x < positionB.x + boundsB.x) &&
		(positionA.x + boundsA.x > positionB.x) &&
		(positionA.y < positionB.y + boundsB.y) &&
		(positionA.y + boundsA.y > positionB.y);
}


void Rinigin::Physics::DetectCollisions()
{
	for (auto* collider : m_Colliders) {
		GameObject* colliderOwner{ collider->GetOwner() };
		if (!colliderOwner or !colliderOwner->IsActive()) continue;
		if (!collider->IsTrigger()) continue;
		if (!collider->IsEnabled()) continue;

		for (auto* other : m_Colliders) { 
			if (!other->IsEnabled()) continue;
			if (other == collider) continue;
			if (collider->IsLayerExcluded(other->GetCollisionLayer())) continue;
			if (other->IsLayerExcluded(collider->GetCollisionLayer())) continue;

			GameObject* otherOwner{ other->GetOwner() };
			if (!otherOwner or !otherOwner->IsActive()) continue;
			if (!other->IsTrigger()) continue;

			const bool wasColliding = collider->IsTouching(other);
			const bool currentlyColliding = AreCollidersOverlapping(collider, other);

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

void Rinigin::Physics::SolveCollisions()
{
	for (auto* rigidbodyA : m_Rigidbodies) {
		GameObject* ownerA{ rigidbodyA->GetOwner() };
		if (!ownerA || !ownerA->IsActive()) continue;
		if (!rigidbodyA->CanCollide()) continue;

		for (auto* rigidbodyB : m_Rigidbodies) {
			if (rigidbodyA == rigidbodyB || !rigidbodyB->CanCollide()) continue;

			GameObject* ownerB{ rigidbodyB->GetOwner() };
			if (!ownerB || !ownerB->IsActive()) continue;

			ColliderComponent* colliderA = rigidbodyA->GetCollider();
			ColliderComponent* colliderB = rigidbodyB->GetCollider();
			if (!colliderA || !colliderB || !colliderA->IsEnabled() || !colliderB->IsEnabled()) continue;
			if (colliderA->IsTrigger() || colliderB->IsTrigger()) continue;
			if (colliderA->IsLayerExcluded(colliderB->GetCollisionLayer())) continue;
			if (colliderB->IsLayerExcluded(colliderA->GetCollisionLayer())) continue;
			if (!AreCollidersOverlapping(colliderA, colliderB)) continue;

			glm::vec3 delta = colliderA->GetCenter() - colliderB->GetCenter();
			glm::vec3 totalHalfSize = colliderA->GetHalfExtents() + colliderB->GetHalfExtents();
			glm::vec3 overlap = totalHalfSize - glm::abs(delta);

			if (overlap.x <= 0.f || overlap.y <= 0.f) continue;

			glm::vec3 correction{};
			if (overlap.x < overlap.y) {
				correction.x = (delta.x > 0.f ? 1.f : -1.f) * overlap.x;
			}
			else {
				correction.y = (delta.y > 0.f ? 1.f : -1.f) * overlap.y;
			}

			// Mass logic
			float massA = !rigidbodyA->IsKinematic() ? rigidbodyA->Mass() : 0.f;
			float massB = !rigidbodyB->IsKinematic() ? rigidbodyB->Mass() : 0.f;
			float totalMass = massA + massB;

			glm::vec3 lockA = rigidbodyA->AxisLock();
			glm::vec3 lockB = rigidbodyB->AxisLock();

			if (massA > 0.f && massB == 0.f) {
				glm::vec3 moveA = correction * lockA;
				ownerA->SetPosition(ownerA->GetWorldPosition() + moveA);
			}
			else if (massB > 0.f && massA == 0.f) {
				glm::vec3 moveB = -correction * lockB;
				ownerB->SetPosition(ownerB->GetWorldPosition() + moveB);
			}
			else if (massA > 0.f && massB > 0.f) {
				glm::vec3 moveA = correction * (massB / totalMass) * lockA;
				glm::vec3 moveB = -correction * (massA / totalMass) * lockB;
				ownerA->SetPosition(ownerA->GetWorldPosition() + moveA);
				ownerB->SetPosition(ownerB->GetWorldPosition() + moveB);
			}
		}
	}
}


bool Rinigin::Physics::IsSolid(int x, int y) const
{
	for (auto* mask : m_CollisionMasks) {
		if (mask->IsSolidAt(x, y)) return true;
	}

	return false;
}

bool Rinigin::Physics::IsOverlappingWithMasks(glm::vec3 position, glm::vec3 bounds) const
{
	int xStart = static_cast<int>(position.x);
	int yStart = static_cast<int>(position.y);
	int xEnd = static_cast<int>(position.x + bounds.x);
	int yEnd = static_cast<int>(position.y + bounds.y);

	bool colliding = false;

	for (int x = xStart; x < xEnd; x++) {
		for (int y = yStart; y < yEnd; y++) {
			if (IsSolid(x, y)) { colliding = true; };
		}
	}

	return colliding;
}

float Rinigin::Physics::GetMaskCoverage(glm::vec3 position, glm::vec3 bounds) const
{
	int xStart = static_cast<int>(position.x);
	int yStart = static_cast<int>(position.y);
	int xEnd = static_cast<int>(position.x + bounds.x);
	int yEnd = static_cast<int>(position.y + bounds.y);

	int solidCount = 0;
	int total = 0;

	for (int x = xStart; x < xEnd; ++x) {
		for (int y = yStart; y < yEnd; ++y) {
			total++;
			if (IsSolid(x, y)) solidCount++;
		}
	}

	if (total == 0) return 0.0f; // avoid division by zero
	return static_cast<float>(solidCount) / static_cast<float>(total);
}


