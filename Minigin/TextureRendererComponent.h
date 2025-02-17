#pragma once
#include <string>
#include "Component.h"
#include "Texture2D.h"

namespace dae {
	class GameObject;
	class Texture2D;
	class TextureRendererComponent final : public Component
	{
	public:
		explicit TextureRendererComponent(GameObject* gameObject);
		const Texture2D* GetTexture() const { return m_Texture; };
		void SetTexture(const std::string& filename);
		void SetTexture(Texture2D* texture);

		virtual void Render() const override;
	private:
		Texture2D* m_Texture;
	};
}


