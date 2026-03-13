#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Game;

	struct Rock
	{
		Position2D position;
		sf::Sprite sprite;
	};

	void InitRocks(Rock& rocks, const Game& game);

	void DrawRocks(Rock& rocks, sf::RenderWindow& window);
}


