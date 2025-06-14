#pragma once
#include "Component.h"
#include <vector>
#include <glm/glm.hpp>
#include <memory>

class Level;
class Pathfinder;
class Character;
class Nobbin;

namespace Rinigin { class GameObject; class ColliderComponent; }

class NobbinAIComponent : public Rinigin::Component
{
public:
    NobbinAIComponent(Rinigin::GameObject* gameObject,Nobbin* nobbin);
    void Update() override;
    void Render() const override;
private:
    Character* FindClosestPlayer() const;
    Nobbin* m_Nobbin;
};
