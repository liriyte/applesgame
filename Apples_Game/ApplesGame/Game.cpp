#include "Game.h"
#include "Math.h"
#include "Constatnts.h"
#include "Player.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include <vector>

namespace ApplesGame
{
	inline bool Worse(const ScoreEntry& first, const ScoreEntry& second)
	{
		if (first.scoreboardScore != second.scoreboardScore)
		{
			return first.scoreboardScore < second.scoreboardScore;
		}

		return first.scoreboardName > second.scoreboardName;
	}

	static void InsertionSort(ScoreEntry* arr, int highCount)
	{
		for (int i = 1; i < highCount; ++i)
		{
			ScoreEntry key = arr[i];
			int j = i - 1;

			while (j >= 0 && Worse(arr[j], key))
			{
				arr[j + 1] = arr[j];
				--j;
			}

			arr[j + 1] = key;
		}
	}

	static void RebuildScoreRows(Game& game)
	{
		InsertionSort(game.highScores, game.highCount);

		const sf::FloatRect titleBounds = game.scoresTitle.getLocalBounds();
		game.scoresTitle.setOrigin(
			titleBounds.left + titleBounds.width / 2.f,
			titleBounds.top + titleBounds.height / 2.f
		);
		game.scoresTitle.setPosition(SCREEN_WIDTH / 2.f, 70.f);

		const float startX = SCREEN_WIDTH * 0.5f - 150.f;
		const float startY = 130.f;
		const float lineStep = 34.f;

		game.rowsShown = (game.highCount < MAX_ROWS_SHOWN) ? game.highCount : MAX_ROWS_SHOWN;

		for (int i = 0; i < game.rowsShown; ++i)
		{
			const ScoreEntry& entry = game.highScores[i];
			game.scoreRows[i].setString(
				std::to_string(i + 1) + ". " +
				entry.scoreboardName + " : " +
				std::to_string(entry.scoreboardScore)
			);
			game.scoreRows[i].setPosition(startX, startY + i * lineStep);
		}
	}

	static int GetBotScoreUpperBound(const Game& game)
	{
		const bool isFiniteMode =
			HasMode(game.gameMode, GameSettingsBits::FiniteApples) ||
			HasMode(game.gameMode, GameSettingsBits::AcceleratePlayerFiniteApples);

		if (isFiniteMode)
		{
			return game.initialApplesCount; // возвращаем количество яблок на старте
		}

		return MAX_RAND_SCORE;
	}

	static void CollectNearbyAppleIndices(const Game& game, std::vector<int>& outAppleIndices)
	{
		outAppleIndices.clear();

		const AppleCellCoord playerCell = GetAppleCellCoord(game.player.position);

		for (int cellY = playerCell.y - 1; cellY <= playerCell.y + 1; ++cellY)
		{
			if (cellY < 0)
			{
				continue;
			}

			auto rowIt = game.applesGrid.find(cellY);
			if (rowIt == game.applesGrid.end())
			{
				continue;
			}

			for (int cellX = playerCell.x - 1; cellX <= playerCell.x + 1; ++cellX)
			{
				if (cellX < 0)
				{
					continue;
				}

				auto cellIt = rowIt->second.find(cellX);
				if (cellIt == rowIt->second.end())
				{
					continue;
				}

				outAppleIndices.insert(
					outAppleIndices.end(),
					cellIt->second.begin(),
					cellIt->second.end()
				);
			}
		}
	}

	void EnsureScoreTableInitialized(Game& game)
	{
		if (game.scoreTableInitialized)
		{
			return;
		}

		const char* names[] = { "Neo", "Lena", "Ghost", "Nora", "Sonic" };
		const int namesCount = static_cast<int>(sizeof(names) / sizeof(names[0]));

		game.highCount = 0;

		for (int i = 0; i < namesCount && game.highCount < MAX_SCORES - 1; ++i)
		{
			game.highScores[game.highCount].scoreboardName = names[i];

			const int botMaxScore = GetBotScoreUpperBound(game);
			const int botMinScore = (MIN_RAND_SCORE < botMaxScore) ? MIN_RAND_SCORE : botMaxScore;
			game.highScores[game.highCount].scoreboardScore =
				botMinScore + (rand() % (botMaxScore - botMinScore + 1));
			++game.highCount;
		}

		if (game.highCount < MAX_SCORES)
		{
			game.highScores[game.highCount].scoreboardName = "Player";
			game.highScores[game.highCount].scoreboardScore = 0;
			++game.highCount;
		}

		game.scoresTitle.setFont(game.font);
		game.scoresTitle.setCharacterSize(30);
		game.scoresTitle.setFillColor(sf::Color::White);
		game.scoresTitle.setString(L"Таблица рекордов");

		for (int i = 0; i < MAX_ROWS_SHOWN; ++i)
		{
			game.scoreRows[i].setFont(game.font);
			game.scoreRows[i].setCharacterSize(22);
			game.scoreRows[i].setFillColor(sf::Color::White);
		}

		game.scoresBackHint.setFont(game.font);
		game.scoresBackHint.setCharacterSize(18);
		game.scoresBackHint.setFillColor(sf::Color::White);
		game.scoresBackHint.setString(L"Нажмите Esc, чтобы вернуться в меню");

		const sf::FloatRect hintBounds = game.scoresBackHint.getLocalBounds();
		game.scoresBackHint.setOrigin(
			hintBounds.left + hintBounds.width / 2.f,
			hintBounds.top + hintBounds.height / 2.f
		);
		game.scoresBackHint.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 35.f);

		RebuildScoreRows(game);
		game.scoreTableInitialized = true;
	}

	void OnGameOver(Game& game)
	{
		EnsureScoreTableInitialized(game);

		int playerIndex = -1;
		for (int i = 0; i < game.highCount; ++i)
		{
			if (game.highScores[i].scoreboardName == "Player")
			{
				playerIndex = i;
				break;
			}
		}

		const int playerScore = game.numEatenApples;

		// Оставляем лучший результат игрока, а не просто перезаписываем его любым значением.
		if (playerIndex != -1)
		{
			if (playerScore > game.highScores[playerIndex].scoreboardScore)
			{
				game.highScores[playerIndex].scoreboardScore = playerScore;
			}
		}
		else if (game.highCount < MAX_SCORES)
		{
			game.highScores[game.highCount].scoreboardName = "Player";
			game.highScores[game.highCount].scoreboardScore = playerScore;
			++game.highCount;
		}

		RebuildScoreRows(game);
	}

	void DrawScoreTable(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.scoresTitle);

		for (int i = 0; i < game.rowsShown; ++i)
		{
			window.draw(game.scoreRows[i]);
		}
	}

	void RestartGame(Game& game)
	{
		InitPlayer(game.player, game);
		game.countApples = GetRansomApplesCount(MIN_NUM_APPLES, MAX_NUM_APPLES);
		game.initialApplesCount = game.countApples; // передаем на старте раунда количество яблок на карте (нужно для режимов с ограниченным числом яблок)

		for (int i = 0; i < game.countApples; ++i)
		{
			InitApples(game.apples[i], game);
		}

		RebuildApplesGrid(game);

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRocks(game.rocks[i], game);
		}

		game.background.setFillColor(sf::Color::Black);
		game.background.setPosition(0.f, 0.f);
		game.scoreText.setFillColor(sf::Color::Red);
		game.textPauseInGame.setFillColor(sf::Color::Red);


		game.numEatenApples = 0;
		game.currentTime = 0.f;
		game.pauseTimeLeft = 0.f;
		game.scoreSavedForCurrentRound = false;
	}

	void InitGame(Game& game)
	{
		assert(game.playerEatApple.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.playerDeath.loadFromFile(RESOURCES_PATH + "\\Death.wav"));

		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.applesTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rocksTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));

		assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"));

		game.background.setSize(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		game.background.setPosition(0.f, 0.f);
		game.background.setFillColor(sf::Color::Black);

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(30);
		game.scoreText.setFillColor(sf::Color::Red);
		game.scoreText.setPosition(1.f, 1.f);
		game.scoreText.setString(L"Your score is: 0");

		game.textPauseInGame.setFont(game.font);
		game.textPauseInGame.setCharacterSize(30);
		game.textPauseInGame.setFillColor(sf::Color::Red);
		game.textPauseInGame.setPosition(1.f, 40.f);
		game.textPauseInGame.setString(L"Пауза = Esc");

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(50);
		game.gameOverText.setFillColor(sf::Color::Black);
		game.gameOverText.setString("Game Over!");
		game.gameOverTextFormSize = game.gameOverText.getLocalBounds();
		game.gameOverText.setOrigin(
			game.gameOverTextFormSize.left + game.gameOverTextFormSize.width / 2.f,
			game.gameOverTextFormSize.top + game.gameOverTextFormSize.height / 2.f - 100.f
		);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		game.youWinText.setFont(game.font);
		game.youWinText.setCharacterSize(60);
		game.youWinText.setFillColor(sf::Color::Black);
		game.youWinText.setString("YOU WIN!!!");
		game.youWinTextFormSize = game.youWinText.getLocalBounds();
		game.youWinText.setOrigin(
			game.youWinTextFormSize.left + game.youWinTextFormSize.width / 2.f,
			game.youWinTextFormSize.top + game.youWinTextFormSize.height / 2.f
		);
		game.youWinText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 70.f);

		game.state = GameState::MainMenu;

		if (game.scoreTableInitialized == false)
		{
			game.textPlayToSeeScoreTable.setFont(game.font);
			game.textPlayToSeeScoreTable.setCharacterSize(25);
			game.textPlayToSeeScoreTable.setFillColor(sf::Color::White);
			game.textPlayToSeeScoreTable.setString(L"Начните игру,\nчтобы попасть в таблицу рекордов");
			game.textPlayToSeeScoreTableBound = game.textPlayToSeeScoreTable.getLocalBounds();
			game.textPlayToSeeScoreTable.setOrigin(
				game.textPlayToSeeScoreTableBound.left + game.textPlayToSeeScoreTableBound.width / 2.f,
				game.textPlayToSeeScoreTableBound.top + game.textPlayToSeeScoreTableBound.height / 2.f
			);
			game.textPlayToSeeScoreTable.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		}


		RestartGame(game);
	}

	void StartGame(Game& game, GameSettingsBits mode)
	{
		game.gameMode = mode;
		RestartGame(game);

		game.scoreTableInitialized = false; // запуск пустой таблицы рекордов (т.к. игрок еще не играл)
		EnsureScoreTableInitialized(game);
		
		game.state = GameState::Playing;
	}

	void StartGameOver(Game& game)
	{
		if (game.state == GameState::GameOver)
		{
			return;
		}

		game.pauseTimeLeft = game.currentTime;
		game.state = GameState::GameOver;

		game.background.setFillColor(sf::Color::Red);
		game.scoreText.setFillColor(sf::Color::Black);

		if (!game.scoreSavedForCurrentRound)
		{
			OnGameOver(game);
			game.scoreSavedForCurrentRound = true;
		}
	}

	void UpdateGame(Game& game, float deltaTime)
	{
		if (game.state == GameState::MainMenu ||
			game.state == GameState::PauseMenu ||
			game.state == GameState::ScoreTable)
		{
			return;
		}

		if (game.state == GameState::GameOver)
		{
			if (game.currentTime - game.pauseTimeLeft > PAUSE_TIME)
			{
				game.state = GameState::MainMenu;
			}
			return;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			game.player.direction = PlayerDirection::Right;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			game.player.direction = PlayerDirection::Down;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			game.player.direction = PlayerDirection::Left;
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			game.player.direction = PlayerDirection::Up;
		}

		switch (game.player.direction)
		{
		case PlayerDirection::Right:
			game.player.position.x += game.player.speed * deltaTime;
			break;

		case PlayerDirection::Down:
			game.player.position.y += game.player.speed * deltaTime;
			break;

		case PlayerDirection::Left:
			game.player.position.x -= game.player.speed * deltaTime;
			break;

		case PlayerDirection::Up:
			game.player.position.y -= game.player.speed * deltaTime;
			break;
		}

		if (
			game.player.position.x - PLAYER_SIZE / 2.f < 0.f ||
			game.player.position.x + PLAYER_SIZE / 2.f > SCREEN_WIDTH ||
			game.player.position.y - PLAYER_SIZE / 2.f < 0.f ||
			game.player.position.y + PLAYER_SIZE / 2.f > SCREEN_HEIGHT)
		{
			PlaySound(game.player, game.playerDeath);
			StartGameOver(game);
			return;
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			if (IsRectengleCollide(
				game.player.position, { PLAYER_SIZE, PLAYER_SIZE },
				game.rocks[i].position, { ROCKS_SIZE, ROCKS_SIZE }))
			{
				PlaySound(game.player, game.playerDeath);
				StartGameOver(game);
				return;
			}
		}

		const bool isFiniteMode =
			HasMode(game.gameMode, GameSettingsBits::FiniteApples) ||
			HasMode(game.gameMode, GameSettingsBits::AcceleratePlayerFiniteApples);

		const bool isAcceleratingMode =
			HasMode(game.gameMode, GameSettingsBits::AcceleratePlayer) ||
			HasMode(game.gameMode, GameSettingsBits::AcceleratePlayerFiniteApples);

		std::vector<int> candidateAppleIndices;
		candidateAppleIndices.reserve(16);

		bool shouldRepeatAppleSearch = true;

		while (shouldRepeatAppleSearch)
		{
			shouldRepeatAppleSearch = false;
			CollectNearbyAppleIndices(game, candidateAppleIndices);

			for (int appleIndex : candidateAppleIndices)
			{
				if (appleIndex < 0 || appleIndex >= game.countApples)
				{
					continue;
				}

				if (!IsApplesCollide(
					game.player.position, PLAYER_SIZE / 2.f,
					game.apples[appleIndex].positions, APPLE_SIZE / 2.f))
				{
					continue;
				}

				++game.numEatenApples;
				PlaySound(game.player, game.playerEatApple);

				if (isAcceleratingMode)
				{
					game.player.speed += ACCELERATION;
				}

				if (isFiniteMode)
				{
					RemoveAppleAt(game, appleIndex);

					if (game.countApples == 0)
					{
						StartGameOver(game);
						return;
					}

					shouldRepeatAppleSearch = true;
				}
				else
				{
					MoveAppleToNewPosition(
						game,
						appleIndex,
						GetRandomPositionInScreen(SCREEN_HEIGHT, SCREEN_WIDTH)
					);
				}

				break;
			}
		}
	}

	void DrawGame(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);

		for (int i = 0; i < game.countApples; ++i)
		{
			DrawApples(game.apples[i], window);
		}

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			DrawRocks(game.rocks[i], window);
		}

		DrawPlayer(game.player, window);
		window.draw(game.scoreText);
		window.draw(game.textPauseInGame);


		if (game.state == GameState::GameOver)
		{
			window.draw(game.gameOverText);

			if (game.countApples == 0)
			{
				window.draw(game.youWinText);
			}

			DrawScoreTable(game, window);
		}
	}

	void DrawScoreTableScreen(Game& game, sf::RenderWindow& window)
	{
		DrawScoreTable(game, window);
		window.draw(game.scoresBackHint);

		if (game.scoreTableInitialized == false)
		{
			window.draw(game.textPlayToSeeScoreTable);
		}
	}

	void DeinitializeGame(Game& game)
	{
	}
}