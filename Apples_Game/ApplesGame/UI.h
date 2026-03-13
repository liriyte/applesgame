#pragma once
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	struct Menu
	{
		const float BUTTON_WIDTH = 280.f;
		const float BUTTON_HEIGHT = 60.f;
		const float GAP_BETWEEN_BUTTON = 20.f;

		sf::Font font;

		// Главное меню
		sf::Text mainTitle;

		sf::RectangleShape menuButton1;
		sf::RectangleShape menuButton2;
		sf::RectangleShape menuButton3;
		sf::RectangleShape menuButton4;
		sf::RectangleShape menuButton5;
		sf::RectangleShape menuButton6;


		sf::Text textFiniteApples;
		sf::Text textInfiniteApples;
		sf::Text textAcceleratePlayer;
		sf::Text textAcceleratePlayerFiniteApples;
		sf::Text textScoreTable;
		sf::Text textExitGame;

		// Меню паузы
		sf::RectangleShape pauseButton1;
		sf::RectangleShape pauseButton2;

		sf::Text pauseTitle;
		sf::Text textContinue;
		sf::Text textExitToMenu;
	};

	void InitMenu(Menu& menu);
	void DrawMainMenu(Menu& menu, sf::RenderWindow& window);
	void DrawPauseMenu(Menu& menu, sf::RenderWindow& window);
}