#pragma once
#include "Character.h"
#include "Prototype.h"
#include "Observer.h"

namespace Rinigin { class StateContextComponent; class Event; }
class Level;
class NobbinPursueState;
class NobbinIdleState;

class Nobbin final : public Character,public Prototype<Nobbin>,public Rinigin::Observer
{
public:
	explicit Nobbin(Level* level);
	Nobbin* Clone() override;

	Rinigin::StateContextComponent* GetStateContext() const { return m_NobbinStateContext; }
	NobbinPursueState* GetPursueState() const { return m_PursueState; };
	NobbinIdleState* GetIdleState() const { return m_IdleState; };
	Level* GetLevel() const { return m_Level; }

	Rinigin::Event* GetEnemyDiedEvent() const { return m_EnemyDied.get(); }

	void Notify(Rinigin::EventArguments& arguments) override;
private:
	void RemoveEnemy();

	Rinigin::StateContextComponent* m_NobbinStateContext;
	NobbinPursueState* m_PursueState;
	NobbinIdleState* m_IdleState;
	Level* m_Level;

	std::unique_ptr<Rinigin::Event> m_EnemyDied;
};

