#pragma once
#include "Component.h"

namespace Rinigin { class TextComponent; };

class FPSComponent final : public Rinigin::Component
{
public: 
	explicit FPSComponent(Rinigin::GameObject* gameObject, Rinigin::TextComponent* textComponent);
	virtual void Update() override;
private:
	Rinigin::TextComponent* m_TextComponent; // Linked text component
};


  