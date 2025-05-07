#pragma once
#include "Component.h"

namespace Rinigin {
	class TextureComponent;
	class TextureRendererComponent;

	enum class SpriteDisplayMode {
		Full,
		Tile
	};

	class SpriteSheetComponent final : public Component
	{
	public:
		explicit SpriteSheetComponent(GameObject* gameObject, TextureRendererComponent* textureRendererComponent, TextureComponent* textureComponent);
		virtual ~SpriteSheetComponent() = default;

		void SetTileWidth(float width) { m_TileWidth = width; UpdateSpriteSheet(); }
		void SetTileHeight(float height) { m_TileHeight = height; UpdateSpriteSheet(); }

		void SetRows(int row) { m_Rows = row; UpdateSpriteSheet(); };
		void SetColumns(int column) { m_Columns = column; UpdateSpriteSheet(); };

		void SetCurrentRow(int row) { m_Row = row; UpdateSpriteSheet(); };
		void SetCurrentColumn(int column) { m_Column = column; UpdateSpriteSheet(); };
		void SetDisplayMode(SpriteDisplayMode displayMode) { m_DisplayMode = displayMode; UpdateSpriteSheet(); }

		int GetRows() const { return m_Rows; };
		int GetColumns() const { return m_Columns; };

		int GetCurrentRow() const { return m_Row; };
		int GetCurrentColumn() const { return m_Column; };
		SpriteDisplayMode GetDisplayMode() const { return m_DisplayMode; }

	private:
		void UpdateFrame(int rows,int columns);
		void UpdateSpriteSheet();
		TextureRendererComponent* m_Renderer;
		TextureComponent* m_TextureComponent;

		SpriteDisplayMode m_DisplayMode;

		float m_TileWidth;
		float m_TileHeight;

		int m_Row;
		int m_Column;

		int m_Rows;
		int m_Columns;
	};
}

