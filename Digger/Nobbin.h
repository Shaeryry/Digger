#pragma once
#include "Character.h"
#include "Prototype.h"

class Level;
class Nobbin final : public Character,public Prototype<Nobbin>
{
public:
	explicit Nobbin(Level* level);
	Nobbin* Clone() override;
private:
	Level* m_Level;
	float m_Speed = 100.f;
};

