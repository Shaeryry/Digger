#pragma once
#include "Event.h"

namespace Rinigin { class GameObject; };
class ScoreComponent;

class GameObjectEventArguments : public Rinigin::EventArguments {
	public:
		explicit GameObjectEventArguments(const char* eventName, Rinigin::GameObject* gameObject);
		Rinigin::GameObject* GetGameObject() const { return m_GameObject; };
	private:
		Rinigin::GameObject* m_GameObject; 
}; 

class ScoreArguments : public Rinigin::EventArguments {
public:
	explicit ScoreArguments(const char* eventName,int addedValue);
	int GetScore() const { return m_AddedValue; };
private:
	int m_AddedValue;
};