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
    NobbinAIComponent(Rinigin::GameObject* gameObject,Nobbin* nobbin, Level* level);
    void Update() override;
    void Render() const override;
private:
    Character* FindClosestPlayer() const;
    void MoveAlongPath();
    void UpdatePath();

    std::unique_ptr<Pathfinder> m_Pathfinder;
    std::vector<glm::vec2> m_Path;

    float m_PathCooldownTimer;
    float m_PathCooldownDuration;
    size_t m_CurrentPathIndex;

    Level* m_Level;
    Nobbin* m_Nobbin;
};
