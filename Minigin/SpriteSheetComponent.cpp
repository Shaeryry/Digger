#include "SpriteSheetComponent.h"
#include "TextureRendererComponent.h"

Rinigin::SpriteSheetComponent::SpriteSheetComponent(GameObject* gameObject, TextureRendererComponent* textureRendererComponent, TextureComponent* texture) :
	Component(gameObject),

	m_DisplayMode(SpriteDisplayMode::Full),

	m_Renderer(textureRendererComponent),  
	m_TextureComponent(texture),

	m_TileWidth(-1),
	m_TileHeight(-1),

	m_Row(0),
	m_Column(0),
	 
	m_Rows(0),  
	m_Columns(0)
{
	m_Renderer->UseSourceRect(true);
}

void Rinigin::SpriteSheetComponent::UpdateFrame(int rows, int columns)
{
	const glm::vec2 imageSize{ m_Renderer->GetTexture()->GetSize() };
	const float srcTileWidth = rows > 0 ? (imageSize.x / rows) : imageSize.x;
	const float srcTileHeight = columns > 0 ? (imageSize.y / columns) : imageSize.y;

	const float tileWidth = m_TileWidth > -1 ? m_TileWidth : srcTileWidth;
	const float tileHeight = m_TileHeight > -1 ? m_TileHeight : srcTileHeight;

	const float tileX = rows > 0 ? (srcTileWidth * m_Row) : 0;
	const float tileY = columns > 0 ? (srcTileHeight * m_Column) : 0;

	m_Renderer->SetSourcePosition(tileX, tileY);
	m_Renderer->SetSourceSize(srcTileWidth, srcTileHeight);

	m_Renderer->SetRectSize(tileWidth, tileHeight);
}

void Rinigin::SpriteSheetComponent::UpdateSpriteSheet()
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
