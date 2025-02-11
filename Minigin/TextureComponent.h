#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include "Texture2D.h"

namespace dae {
	class Font;
	class TextureComponent : public Component
	{
	public:
		TextureComponent(const std::string& filename);

		void SetTexture(const std::string& filename);
		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}