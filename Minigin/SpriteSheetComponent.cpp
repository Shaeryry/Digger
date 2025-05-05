#include "SpriteSheetComponent.h"
#include "TextureRendererComponent.h"

Rinigin::SpriteSheetComponent::SpriteSheetComponent(GameObject* gameObject, TextureRendererComponent* textureRendererComponent, TextureComponent* texture) :
	Component(gameObject),

	m_DisplayMode(SpriteDisplayMode::Full),

	m_Renderer(textureRendererComponent),  
	m_TextureComponent(texture),

	m_Row(0),
	m_Column(0),
	 
	m_Rows(0),  
	m_Columns(0)
{
}

void Rinigin::SpriteSheetComponent::UpdateFrame(int /*rows*/, int /*columns*/)
{
	//const glm::vec2 imageSize{ m_Renderer->GetTexture()->GetSize() };
	//const float tileWidth = rows > 0 ? (imageSize.x / rows) : imageSize.x;
	//const float tileHeight = columns > 0 ? (imageSize.y / columns) : imageSize.y;
	///*const float tileX = rows > 0 ? (tileWidth * m_Row) : 0;
	//const float tileY = columns > 0 ? (tileHeight * m_Column) : 0;*/

	//m_Renderer->SetRect(200, 200); // Set tile rect
	//m_Renderer->SetOffset(tileX, tileY); // Set position
}

void Rinigin::SpriteSheetComponent::UpdateDisplayMode()
{
	switch (m_DisplayMode)
	{
	case Rinigin::SpriteDisplayMode::Full:
		UpdateFrame(0, 0);
		break; 
	case Rinigin::SpriteDisplayMode::Tile:
		UpdateFrame(m_Rows, m_Columns);
		break;
	default:
		break;
	}
}
