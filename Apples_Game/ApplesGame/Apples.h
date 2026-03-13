#pragma once
#include "Math.h"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>

namespace ApplesGame
{
	struct Game;

	struct Apple
	{
		Position2D positions;
		sf::Sprite sprite;
	};

	struct AppleCellCoord
	{
		int x = 0;
		int y = 0;
	};

	using AppleIndexSet = std::unordered_set<int>;
	using ApplesGridRow = std::unordered_map<int, AppleIndexSet>;
	using ApplesGrid = std::unordered_map<int, ApplesGridRow>;

	void InitApples(Apple& apples, const Game& game);
	void DrawApples(Apple& apples, sf::RenderWindow& window);

	int GetAppleCellX(Position2D position);
	int GetAppleCellY(Position2D position);
	AppleCellCoord GetAppleCellCoord(Position2D position);

	void RebuildApplesGrid(Game& game);
	void MoveAppleToNewPosition(Game& game, int appleIndex, Position2D newPosition);
	void RemoveAppleAt(Game& game, int appleIndex);
}