#pragma once
#include <string>
#include <glm.hpp>
#include "Component.h"
#include "Texture2D.h"

namespace Rinigin {
	struct RendererRect {
		float x;
		float y;
		float w;
		float h;
	};

	class GameObject;
	class Texture2D;
	class TextureRendererComponent final : public Component
	{
	public:
		explicit TextureRendererComponent(GameObject* gameObject);
		const Texture2D* GetTexture() const { return m_Texture; };

		void SetTexture(const std::string& filename);
		void SetTexture(Texture2D* texture);

		void SetWidth(float width) { m_Rect.w = width; }; 
		void SetHeight(float height) { m_Rect.h = height; };

		virtual void Render() const override;
	private:
		RendererRect m_Rect;
		RendererRect m_SrcRect;
		Texture2D* m_Texture;
	};
} 


