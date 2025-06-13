#pragma once
#include <State.h>

class Nobbin;
class NobbinPursueState : public Rinigin::State
{
public:
	explicit NobbinPursueState(Rinigin::StateContextComponent* context, Nobbin* nobbin);
private:
	Nobbin* m_Nobbin;
};

