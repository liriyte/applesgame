#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Math.h"
#include "Constatnts.h"

namespace ApplesGame
{
	enum PlayerDirection // increase number +1 after the first number
	{
		Right = 0,
		Up,
		Left,
		Down
	};

	struct Game;

	struct Player
	{
		Position2D position;
		float speed = INITIAL_SPEED;
		PlayerDirection direction = PlayerDirection::Right;
		float spriteScale = 1.f;
		sf::Sprite sprite;

		sf::Sound sound;
	};

	void InitPlayer(Player& player, const Game& game);

	void DrawPlayer(Player& player, sf::RenderWindow& window);

	void PlaySound(Player& player, sf::SoundBuffer& buffer);
}



