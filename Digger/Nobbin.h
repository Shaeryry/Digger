#pragma once
#include "Character.h"
#include "Prototype.h"
#include "Observer.h"

class Level;
class Nobbin final : public Character,public Prototype<Nobbin>,public Rinigin::Observer
{
public:
	explicit Nobbin(Level* level);
	Nobbin* Clone() override;

	void Notify(Rinigin::EventArguments& arguments) override;
private:
	Level* m_Level;
};

