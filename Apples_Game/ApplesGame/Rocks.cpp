#include "Rocks.h"
#include "Constatnts.h"
#include "Game.h"


namespace ApplesGame
{

	void InitRocks(Rock& rocks, const Game& game)
	{
		rocks.sprite.setTexture(game.rocksTexture);
		SetSpriteSize(rocks.sprite, ROCKS_SIZE, ROCKS_SIZE);
		SetSpriteRelativeOrigin(rocks.sprite, 0.5f, 0.5f);
		rocks.position = GetRandomPositionInScreen(SCREEN_HEIGHT, SCREEN_WIDTH);
	}

	void DrawRocks(Rock& rocks, sf::RenderWindow& window)
	{
		rocks.sprite.setPosition(rocks.position.x, rocks.position.y); 
		window.draw(rocks.sprite);
	}
}


