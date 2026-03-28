#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <ctime>
#include <cstdlib>

#include "Game.h"
#include "UI.h"

int main()
{
	using namespace ApplesGame;

	int seed = (int)time(nullptr);
	srand(seed);

	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Apples Game");

	Menu menu;
	Game game;

	InitGame(game);
	InitMenu(menu);

	sf::Clock gameClock;
	float lastTime = gameClock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{
		game.currentTime = gameClock.getElapsedTime().asSeconds();
		float deltaTime = game.currentTime - lastTime;
		lastTime = game.currentTime;

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
				break;
			}

			if (event.type == sf::Event::KeyPressed)
			{
				using sf::Keyboard;

				switch (game.state)
				{
				case GameState::MainMenu:
				{
					
					switch (event.key.code)
					{
					case Keyboard::Num1:
						StartGame(game, GameSettingsBits::FiniteApples);
						break;

					case Keyboard::Num2:
						StartGame(game, GameSettingsBits::InfiniteApples);
						break;

					case Keyboard::Num3:
						StartGame(game, GameSettingsBits::AcceleratePlayer);
						break;

					case Keyboard::Num4:
						StartGame(game, GameSettingsBits::AcceleratePlayerFiniteApples);
						break;

					case Keyboard::Num5:
						game.scoreTableScreenId = ScoreTableId::InfiniteApples;
						game.state = GameState::ScoreTable;
						break;

					case Keyboard::Num6:
					case Keyboard::Escape:
						window.close();
						break;

					default:
						break;
					}
					break;
				}

				case GameState::Playing:
				{
					if (event.key.code == Keyboard::Escape)
					{
						game.state = GameState::PauseMenu;
					}
					break;
				}

				case GameState::PauseMenu:
				{
					if (event.key.code == Keyboard::Num1)
					{
						game.state = GameState::Playing;
					}
					else if (event.key.code == Keyboard::Num2)
					{
						game.state = GameState::MainMenu;
					}
					break;
				}

				case GameState::ScoreTable: // переключение таблиц рекордов внутри меню
				{
					if (event.key.code == Keyboard::Num1)
					{
						game.scoreTableScreenId = ScoreTableId::FiniteApples;
					}
					else if (event.key.code == Keyboard::Num2)
					{
						game.scoreTableScreenId = ScoreTableId::InfiniteApples;
					}
					else if (event.key.code == Keyboard::Num3)
					{
						game.scoreTableScreenId = ScoreTableId::AcceleratePlayer;
					}
					else if (event.key.code == Keyboard::Num4)
					{
						game.scoreTableScreenId = ScoreTableId::AcceleratePlayerFiniteApples;
					}
					else if (event.key.code == Keyboard::Escape)
					{
						game.state = GameState::MainMenu;
					}
					break;
				}

				case GameState::GameOver:
				{
					// Во время Game Over ничего не делаем:
					// ждём окончание паузы и автоматический возврат в меню.
					break;
				}
				}
			}
		}

		game.scoreText.setString("Your score is: " + std::to_string(game.numEatenApples));

		UpdateGame(game, deltaTime);

		window.clear(sf::Color::Black);

		switch (game.state)
		{
		case GameState::MainMenu:
			window.draw(game.background); // ПРОВЕРИТЬ! НЕ затягивает ли левый фон
			DrawMainMenu(menu, window);
			break;

		case GameState::Playing:
			DrawGame(game, window);
			break;

		case GameState::PauseMenu:
			DrawGame(game, window);
			DrawPauseMenu(menu, window);
			break;

		case GameState::ScoreTable:
			DrawScoreTableScreen(game, window);
			break;

		case GameState::GameOver:
			DrawGame(game, window);
			break;
		}

		window.display();
	}

	DeinitializeGame(game);
	return 0;
}