#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <cstdint>

#include "Constatnts.h"
#include "Player.h"
#include "Apples.h"
#include "Rocks.h"
#include "UI.h"

namespace ApplesGame
{
	enum class GameState
	{
		MainMenu,
		Playing,
		PauseMenu,
		ScoreTable,
		GameOver
	};

	enum class GameSettingsBits : uint8_t
	{
		None = 0,
		FiniteApples = 1 << 0,
		InfiniteApples = 1 << 1,
		AcceleratePlayer = 1 << 2,
		AcceleratePlayerFiniteApples = 1 << 3
	};

	inline GameSettingsBits operator|(GameSettingsBits a, GameSettingsBits b)
	{
		return static_cast<GameSettingsBits>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline GameSettingsBits& operator|=(GameSettingsBits& a, GameSettingsBits b)
	{
		return a = a | b;
	}

	inline GameSettingsBits operator^(GameSettingsBits a, GameSettingsBits b)
	{
		return static_cast<GameSettingsBits>(
			static_cast<uint8_t>(a) ^ static_cast<uint8_t>(b));
	}

	inline GameSettingsBits& operator^=(GameSettingsBits& a, GameSettingsBits b)
	{
		return a = a ^ b;
	}

	inline bool HasMode(GameSettingsBits mask, GameSettingsBits flag)
	{
		return (static_cast<uint8_t>(mask) & static_cast<uint8_t>(flag)) != 0;
	}

	struct ScoreEntry
	{
		std::string scoreboardName = "";
		int scoreboardScore = 0;
	};

	struct Game
	{
		GameSettingsBits gameMode = GameSettingsBits::None;
		GameState state = GameState::MainMenu;

		int countApples = MAX_NUM_APPLES;
		int initialApplesCount = 0; // сколько яблок инициализируется на старте раунда

		Player player;
		Apple apples[MAX_NUM_APPLES];
		ApplesGrid applesGrid; // добавляю сетку с яблоками
		AppleCellCoord appleCellCoords[MAX_NUM_APPLES] = {}; // макссив клеток с яблоками
		Rock rocks[NUM_ROCKS];

		sf::RectangleShape background;
		sf::FloatRect gameOverTextFormSize;
		sf::FloatRect youWinTextFormSize;

		int numEatenApples = 0;

		float currentTime = 0.f;
		float pauseTimeLeft = 0.f;
		bool scoreSavedForCurrentRound = false;

		// Text / UI
		sf::Font font;
		sf::Text scoreText;
		sf::Text gameOverText;
		sf::Text youWinText;
		sf::Text textPauseInGame;


		// Resources
		sf::Texture playerTexture;
		sf::Texture applesTexture;
		sf::Texture rocksTexture;

		sf::SoundBuffer playerEatApple;
		sf::SoundBuffer playerDeath;

		// Scoreboard
		bool scoreTableInitialized = false;
		ScoreEntry highScores[MAX_SCORES];
		int highCount = 0;
		sf::Text scoresTitle;

		sf::Text textPlayToSeeScoreTable;
		sf::FloatRect textPlayToSeeScoreTableBound;

		sf::Text scoreRows[MAX_ROWS_SHOWN];
		sf::Text scoresBackHint;
		int rowsShown = 0;
	};

	void RestartGame(Game& game);
	void InitGame(Game& game);
	void StartGame(Game& game, GameSettingsBits mode);
	void StartGameOver(Game& game);
	void UpdateGame(Game& game, float deltaTime);
	void DrawGame(Game& game, sf::RenderWindow& window);
	void DrawScoreTableScreen(Game& game, sf::RenderWindow& window);
	void DeinitializeGame(Game& game);

	void EnsureScoreTableInitialized(Game& game);
	void OnGameOver(Game& game);
	void DrawScoreTable(Game& game, sf::RenderWindow& window);
}