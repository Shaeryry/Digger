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
		TextureComponent();
		TextureComponent(const std::string& filename);

		const std::shared_ptr<Texture2D>& GetTexture() const { return m_texture; };
		void SetTexture(const std::string& filename);
		void SetTexture(std::shared_ptr<Texture2D> texture);

		virtual void Render() const override;
	private:
		std::shared_ptr<Texture2D> m_texture;
	};
}