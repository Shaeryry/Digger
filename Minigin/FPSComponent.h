#pragma once
#include "TextComponent.h"

namespace dae {
	class FPSComponent final : public TextComponent
	{
	public: 
		FPSComponent(GameObject* gameObject, Font* font);
		virtual void Update() override;
	};
}

  