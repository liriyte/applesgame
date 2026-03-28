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

	// предварительная настройка таблиц рекордов под каждый режим
	static int ToIndex(ScoreTableId tableId)
	{
		return static_cast<int>(tableId);
	}

	static ScoreTableData& GetScoreTableData(Game& game, ScoreTableId tableId)
	{
		return game.scoreTables[ToIndex(tableId)];
	}

	static const ScoreTableData& GetScoreTableData(const Game& game, ScoreTableId tableId)
	{
		return game.scoreTables[ToIndex(tableId)];
	}

	static ScoreTableId GetScoreTableIdFromMode(GameSettingsBits mode)
	{
		switch (mode)
		{
		case GameSettingsBits::FiniteApples:
			return ScoreTableId::FiniteApples;

		case GameSettingsBits::InfiniteApples:
			return ScoreTableId::InfiniteApples;

		case GameSettingsBits::AcceleratePlayer:
			return ScoreTableId::AcceleratePlayer;

		case GameSettingsBits::AcceleratePlayerFiniteApples:
			return ScoreTableId::AcceleratePlayerFiniteApples;

		default:
			return ScoreTableId::InfiniteApples;
		}
	}

	static const wchar_t* GetScoreTableTitle(ScoreTableId tableId)
	{
		switch (tableId)
		{
		case ScoreTableId::FiniteApples:
			return L"ТАБЛИЦА РЕКОРДОВ:\nконечные яблоки";

		case ScoreTableId::InfiniteApples:
			return L"ТАБЛИЦА РЕКОРДОВ:\nбесконечные яблоки";

		case ScoreTableId::AcceleratePlayer:
			return L"ТАБЛИЦА РЕКОРДОВ:\nускорение игрока";

		case ScoreTableId::AcceleratePlayerFiniteApples:
			return L"ТАБЛИЦА РЕКОРДОВ:\nускорение + лимит яблок";

		default:
			return L"ТАБЛИЦА РЕКОРДОВ";
		}
	}

	static int GetBotScoreUpperBound(ScoreTableId tableId)
	{
		switch (tableId)
		{
		case ScoreTableId::FiniteApples:
		case ScoreTableId::AcceleratePlayerFiniteApples:
			return MAX_NUM_APPLES;

		case ScoreTableId::InfiniteApples:
		case ScoreTableId::AcceleratePlayer:
			return MAX_RAND_SCORE;

		default:
			return MAX_RAND_SCORE;
		}
	}
	
	// инициализация интерфейса таблицы для всех режимов
	static void InitScoreTableUi(Game& game)
	{
		game.scoresTitle.setFont(game.font);
		game.scoresTitle.setCharacterSize(28);
		game.scoresTitle.setFillColor(sf::Color::White);

		for (int i = 0; i < MAX_ROWS_SHOWN; ++i)
		{
			game.scoreRows[i].setFont(game.font);
			game.scoreRows[i].setCharacterSize(22);
			game.scoreRows[i].setFillColor(sf::Color::White);
		}

		game.scoresBackHint.setFont(game.font);
		game.scoresBackHint.setCharacterSize(18);
		game.scoresBackHint.setFillColor(sf::Color::White);
		game.scoresBackHint.setString(L"[1–4]		 сменить таблицу\n[Esc]		 вернуться в меню");

		const sf::FloatRect hintBounds = game.scoresBackHint.getLocalBounds();
		game.scoresBackHint.setOrigin(
			hintBounds.left + hintBounds.width / 2.f,
			hintBounds.top + hintBounds.height / 2.f
		);
		game.scoresBackHint.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT - 55.f);
	}

	// пересобираем счетчик таблицы рекордов
	static void RebuildScoreRows(Game& game, ScoreTableId tableId)
	{
		ScoreTableData& table = GetScoreTableData(game, tableId);

		InsertionSort(table.highScores, table.highCount);

		game.scoresTitle.setString(GetScoreTableTitle(tableId));

		const sf::FloatRect titleBounds = game.scoresTitle.getLocalBounds();
		game.scoresTitle.setOrigin(
			titleBounds.left + titleBounds.width / 2.f,
			titleBounds.top + titleBounds.height / 2.f
		);
		game.scoresTitle.setPosition(SCREEN_WIDTH / 2.f, 70.f);

		const float startX = SCREEN_WIDTH * 0.5f - 150.f;
		const float startY = 130.f;
		const float lineStep = 34.f;

		game.rowsShown = (table.highCount < MAX_ROWS_SHOWN) ? table.highCount : MAX_ROWS_SHOWN;

		for (int i = 0; i < game.rowsShown; ++i)
		{
			const ScoreEntry& entry = table.highScores[i];
			game.scoreRows[i].setString(
				std::to_string(i + 1) + ". " +
				entry.scoreboardName + " : " +
				std::to_string(entry.scoreboardScore)
			);
			game.scoreRows[i].setPosition(startX, startY + i * lineStep);
		}
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

	void EnsureScoreTableInitialized(Game& game, ScoreTableId tableId)
	{
		ScoreTableData& table = GetScoreTableData(game, tableId);

		if (table.initialized)
		{
			return;
		}

		const char* names[] = { "Neo", "Lena", "Ghost", "Nora", "Sonic" };
		const int namesCount = static_cast<int>(sizeof(names) / sizeof(names[0]));

		table.highCount = 0;

		const int botMaxScore = GetBotScoreUpperBound(tableId);
		const int botMinScore = (MIN_RAND_SCORE < botMaxScore) ? MIN_RAND_SCORE : botMaxScore;

		for (int i = 0; i < namesCount && table.highCount < MAX_SCORES - 1; ++i)
		{
			table.highScores[table.highCount].scoreboardName = names[i];
			table.highScores[table.highCount].scoreboardScore =
				botMinScore + (rand() % (botMaxScore - botMinScore + 1));
			++table.highCount;
		}

		if (table.highCount < MAX_SCORES)
		{
			table.highScores[table.highCount].scoreboardName = "Player";
			table.highScores[table.highCount].scoreboardScore = 0;
			++table.highCount;
		}

		table.initialized = true;
	}

	void OnGameOver(Game& game)
	{
		const ScoreTableId tableId = GetScoreTableIdFromMode(game.gameMode);

		EnsureScoreTableInitialized(game, tableId);

		ScoreTableData& table = GetScoreTableData(game, tableId);

		int playerIndex = -1;
		for (int i = 0; i < table.highCount; ++i)
		{
			if (table.highScores[i].scoreboardName == "Player")
			{
				playerIndex = i;
				break;
			}
		}

		const int playerScore = game.numEatenApples;

		if (playerIndex != -1)
		{
			if (playerScore > table.highScores[playerIndex].scoreboardScore)
			{
				table.highScores[playerIndex].scoreboardScore = playerScore;
			}
		}
		else if (table.highCount < MAX_SCORES)
		{
			table.highScores[table.highCount].scoreboardName = "Player";
			table.highScores[table.highCount].scoreboardScore = playerScore;
			++table.highCount;
		}
	}

	void DrawScoreTable(Game& game, sf::RenderWindow& window, ScoreTableId tableId)
	{
		EnsureScoreTableInitialized(game, tableId);
		RebuildScoreRows(game, tableId);

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

		for (int i = 0; i < game.countApples; ++i)
		{
			InitApples(game.apples[i], game);
		}

		RebuildApplesGrid(game);

		for (int i = 0; i < NUM_ROCKS; ++i)
		{
			InitRocks(game.rocks[i], game);
		}

		game.background.setTexture(game.backgroundTexture, true);
		game.background.setPosition(0.f, 0.f);

		game.numEatenApples = 0;
		game.currentTime = 0.f;
		game.pauseTimeLeft = 0.f;
		game.scoreSavedForCurrentRound = false;
	}

	void InitGame(Game& game)
	{
		assert(game.playerEatApple.loadFromFile(RESOURCES_PATH + "\\AppleEat.wav"));
		assert(game.playerDeath.loadFromFile(RESOURCES_PATH + "\\Death.wav"));

		assert(game.backgroundTexture.loadFromFile(RESOURCES_PATH + "\\Background.png"));
		assert(game.backgroundEndTexture.loadFromFile(RESOURCES_PATH + "\\Background_End.png"));
		assert(game.playerTexture.loadFromFile(RESOURCES_PATH + "\\Player.png"));
		assert(game.applesTexture.loadFromFile(RESOURCES_PATH + "\\Apple.png"));
		assert(game.rocksTexture.loadFromFile(RESOURCES_PATH + "\\Rock.png"));

		assert(game.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"));

		game.background.setTexture(game.backgroundTexture);
		game.background.setPosition(0.f, 0.f);

		game.scoreText.setFont(game.font);
		game.scoreText.setCharacterSize(20);
		game.scoreText.setFillColor(sf::Color::White);
		game.scoreText.setPosition(1.f, 1.f);
		game.scoreText.setString(L"Your score is: 0");

		game.textPauseInGame.setFont(game.font);
		game.textPauseInGame.setCharacterSize(20);
		game.textPauseInGame.setFillColor(sf::Color::White);
		game.textPauseInGame.setPosition(1.f, 20.f);
		game.textPauseInGame.setString(L"Пауза — Esc");

		game.gameOverText.setFont(game.font);
		game.gameOverText.setCharacterSize(50);
		game.gameOverText.setFillColor(sf::Color::Red);
		game.gameOverText.setString("Game Over!");
		game.gameOverTextFormSize = game.gameOverText.getLocalBounds();
		game.gameOverText.setOrigin(
			game.gameOverTextFormSize.left + game.gameOverTextFormSize.width / 2.f,
			game.gameOverTextFormSize.top + game.gameOverTextFormSize.height / 2.f - 100.f);
		game.gameOverText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f);

		game.youWinText.setFont(game.font);
		game.youWinText.setCharacterSize(60);
		game.youWinText.setFillColor(sf::Color::Red);
		game.youWinText.setString("YOU WIN!!!");
		game.youWinTextFormSize = game.youWinText.getLocalBounds();
		game.youWinText.setOrigin(
			game.youWinTextFormSize.left + game.youWinTextFormSize.width / 2.f,
			game.youWinTextFormSize.top + game.youWinTextFormSize.height / 2.f
		);
		game.youWinText.setPosition(SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f - 100.f);

		game.state = GameState::MainMenu;
		game.scoreTableScreenId = ScoreTableId::InfiniteApples;

		InitScoreTableUi(game);

		RestartGame(game);

		EnsureScoreTableInitialized(game, ScoreTableId::FiniteApples);
		EnsureScoreTableInitialized(game, ScoreTableId::InfiniteApples);
		EnsureScoreTableInitialized(game, ScoreTableId::AcceleratePlayer);
		EnsureScoreTableInitialized(game, ScoreTableId::AcceleratePlayerFiniteApples);
	}

	void StartGame(Game& game, GameSettingsBits mode)
	{
		game.gameMode = mode;
		RestartGame(game);
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

		game.background.setTexture(game.backgroundEndTexture, true);
		game.background.setPosition(0.f, 0.f);

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
				game.background.setTexture(game.backgroundTexture, true); // обновляю фон в меню
				game.background.setPosition(0.f, 0.f);
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

			DrawScoreTable(game, window, GetScoreTableIdFromMode(game.gameMode));
		}
	}

	void DrawScoreTableScreen(Game& game, sf::RenderWindow& window)
	{
		window.draw(game.background);
		DrawScoreTable(game, window, game.scoreTableScreenId);
		window.draw(game.scoresBackHint);
	}

	void DeinitializeGame(Game& game)
	{
	}
}