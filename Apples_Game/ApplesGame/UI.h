#pragma once
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Menu
	{
		const float BUTTON_WIDTH = 320.f;
		const float BUTTON_HEIGHT = 60.f;
		const float GAP_BETWEEN_BUTTON = 20.f;

		sf::Font font;
		sf::Texture buttonTexture;

		// Главное меню
		sf::Text mainTitle;

		sf::Sprite menuButton1;
		sf::Sprite menuButton2;
		sf::Sprite menuButton3;
		sf::Sprite menuButton4;
		sf::Sprite menuButton5;
		sf::Sprite menuButton6;


		sf::Text textFiniteApples;
		sf::Text textInfiniteApples;
		sf::Text textAcceleratePlayer;
		sf::Text textAcceleratePlayerFiniteApples;
		sf::Text textScoreTable;
		sf::Text textExitGame;

		// Меню паузы
		sf::Sprite pauseButton1;
		sf::Sprite pauseButton2;

		sf::Text pauseTitle;
		sf::Text textContinue;
		sf::Text textExitToMenu;
	};

	void InitMenu(Menu& menu);
	void DrawMainMenu(Menu& menu, sf::RenderWindow& window);
	void DrawPauseMenu(Menu& menu, sf::RenderWindow& window);
}