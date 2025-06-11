#pragma once
#include <string>
#include <memory>
#include "Prototype.h"

namespace Rinigin { 
	class GameObject; 
	class TextureRendererComponent;
	class TextureComponent;
}

class Level;
class Item : public Prototype<Item>
{
public:
	explicit Item(Level* level,const std::string& fileName);
	virtual ~Item();
	Item(const Item& other) = delete;
	Item(Item&& other) = delete;
	Item& operator=(const Item& other) = delete;
	Item& operator=(Item&& other) = delete;

	virtual Item* Clone() override = 0;

	Rinigin::GameObject* GetItemObject() const { return m_ItemObject; }
	Rinigin::TextureRendererComponent* GetRenderer() const { return m_Renderer; }
	Rinigin::TextureComponent* GetTextureComponent() const { return m_TextureComponent; }
protected:
	Rinigin::TextureRendererComponent* m_Renderer;
	Rinigin::TextureComponent* m_TextureComponent;
	Level* m_Level;
private:
	Rinigin::GameObject* m_ItemObject;
};

