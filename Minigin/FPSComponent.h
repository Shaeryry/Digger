#pragma once
#include "TextComponent.h"

namespace dae {
	class FPSComponent final : public TextComponent
	{
	public: 
		FPSComponent(GameObject* gameObject, std::shared_ptr<Font> font);
		virtual void Update() override;
	};
}

 