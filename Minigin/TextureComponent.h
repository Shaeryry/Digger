#pragma once
#include <memory>
#include <string>
#include "Component.h"
#include "Texture2D.h"

namespace dae {
	class TextureComponent : public Component
	{
	public:
		TextureComponent(GameObject* gameObject);
		TextureComponent(GameObject* gameObject, const std::string& filename);

		const Texture2D* GetTexture() const { return m_Texture; };
		void SetTexture(const std::string& filename);
		void SetTexture(Texture2D* texture);

		virtual void Render() const override;
	private:
		Texture2D* m_Texture;
	};
}