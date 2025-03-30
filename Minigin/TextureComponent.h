#pragma once
#include <memory>
#include <string>
#include "Component.h"

namespace Rinigin {
	class Texture2D;
	class TextureRendererComponent;
	class TextureComponent final : public Component
	{
	public:
		explicit TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer);
		explicit TextureComponent(GameObject* gameObject, TextureRendererComponent* textureRenderer, const std::string& filename);
		void SetTexture(const std::string& fileName);
	private:
		TextureRendererComponent* m_Renderer;
	};
}