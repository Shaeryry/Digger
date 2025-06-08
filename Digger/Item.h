#pragma once
#include <string>

namespace Rinigin { 
	class GameObject; 
	class ColliderComponent;
	class TextureRendererComponent;
	class TextureComponent;
	class SpriteSheetComponent;
	class SpriteAnimatorComponent;
}

class Item
{
public:
	explicit Item(const std::string& fileName);
	virtual ~Item() = default;
	Item(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item& operator=(Item&& other) = delete;

	Rinigin::TextureRendererComponent* GetRenderer() const { return m_Renderer; }
	Rinigin::GameObject* GetItemObject() const { return m_ItemObject; }
	Rinigin::ColliderComponent* GetTrigger() const { return m_Trigger; }
	Rinigin::TextureComponent* GetTextureComponent() const { return m_TextureComponent; }
private:
	Rinigin::GameObject* m_ItemObject;
	Rinigin::ColliderComponent* m_Trigger;

	Rinigin::TextureRendererComponent* m_Renderer;
	Rinigin::TextureComponent* m_TextureComponent;
};

