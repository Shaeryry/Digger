#pragma once
#include <SDL.h>
#include <string>
#include <glm.hpp>
#include "Component.h"
#include "Texture2D.h"

namespace Rinigin {
	class GameObject;
	class Texture2D;
	class TextureRendererComponent final : public Component
	{
	public:
		explicit TextureRendererComponent(GameObject* gameObject);
		const Texture2D* GetTexture() const { return m_Texture; };

		void UseSourceRect(bool state) { m_UseSrcRect = state; };
		void SetTexture(const std::string& filename);
		void SetTexture(Texture2D* texture);

		void SetSourceRect(const SDL_Rect& srcRect) { m_SrcRect = srcRect; };
		void SetSourcePosition(float x, float y) { m_SrcRect.x = static_cast<int>(x); m_SrcRect.y = static_cast<int>(y); };
		void SetSourcePosition(const glm::vec2& position) { SetSourcePosition(position.x,position.y); };
		void SetSourceSize(float w, float h) { SetSourceWidth(w); SetSourceHeight(h); };
		void SetSourceSize(const glm::vec2& size) { SetSourceSize(size.x, size.y); };
		void SetSourceWidth(float width) { m_SrcRect.w = static_cast<int>(width); };
		void SetSourceHeight(float height) { m_SrcRect.h = static_cast<int>(height); };


		void SetRect(const SDL_Rect& rect) { m_Rect = rect; };
		void SetRectPosition(float x, float y) { m_Rect.x = static_cast<int>(x); m_Rect.y = static_cast<int>(y); };
		void SetRectPosition(const glm::vec2& position) { SetRectPosition(position.x, position.y); };
		void SetRectSize(float w, float h) { SetRectWidth(w); SetRectHeight(h); };
		void SetRectSize(const glm::vec2& size) { SetRectSize(size.x, size.y); };
		void SetRectWidth(float width) { m_Rect.w = static_cast<int>(width); };
		void SetRectHeight(float height) { m_Rect.h = static_cast<int>(height); };


		virtual void Render() const override;
	private:
		bool m_UseSrcRect;
		SDL_Rect m_Rect;
		SDL_Rect m_SrcRect;
		Texture2D* m_Texture;
	};
} 


