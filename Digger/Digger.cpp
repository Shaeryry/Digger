#include "Digger.h"
#include "TextureComponent.h"
#include "SpriteSheetComponent.h"

Digger::Digger()
{
	GetTextureComponent()->SetTexture("testSprite.jpg");

	// Setup sprite
	Rinigin::SpriteSheetComponent* spriteSheet = GetSpriteSheetComponent();

	spriteSheet->SetRows(9);
	spriteSheet->SetColumns(4);
	spriteSheet->SetDisplayMode(Rinigin::SpriteDisplayMode::Tile);

}
