#pragma once
#include <State.h>
#include <memory>
#include <glm/glm.hpp>
#include <vector>

class Pathfinder;
class Character;
class Nobbin;

class NobbinPursueState : public Rinigin::State
{
public:
	explicit NobbinPursueState(Rinigin::StateContextComponent* context, Nobbin* nobbin);

    virtual void Enter() override;
    virtual Rinigin::State* Update() override;
    virtual void Exit() override;
    
    void SetTarget(glm::vec2 target) { m_Target = target; };
    void DrawPath() const;
private:
    void MoveAlongPath();
    void UpdatePath();
     
    std::unique_ptr<Pathfinder> m_Pathfinder;
    std::vector<glm::vec2> m_Path;

    float m_PathCooldownTimer;
    float m_PathCooldownDuration;
    size_t m_CurrentPathIndex;

    glm::vec2 m_Target;
	Nobbin* m_Nobbin;
};

