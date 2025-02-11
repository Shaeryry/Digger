#pragma once
#include "Component.h"

namespace dae {
	class TextComponent;
	class FPSComponent final : public Component
	{
	public:
		FPSComponent();
		FPSComponent(TextComponent* textComponent);
		void AssignTextComponent(TextComponent* textComponent);
		const TextComponent* GetAssignedTextComponent() const { return m_assignedTextComponent; };

		virtual void Update() override;
	private:
		TextComponent* m_assignedTextComponent;
	};
}

