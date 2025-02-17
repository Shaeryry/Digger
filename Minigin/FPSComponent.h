#pragma once
#include "Component.h"

namespace dae {
	class Font;
	class TextComponent;
	class FPSComponent final : public Component
	{
	public: 
		explicit FPSComponent(GameObject* gameObject, TextComponent* textComponent);
		virtual void Update() override;
	private:
		TextComponent* m_TextComponent; // Linked text component
	};
}

  