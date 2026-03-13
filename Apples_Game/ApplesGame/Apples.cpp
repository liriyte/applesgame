#include "Apples.h"
#include "Constatnts.h"
#include "Game.h"

namespace ApplesGame
{
	static void AddAppleToGrid(Game& game, int appleIndex)
	{
		const AppleCellCoord cell = GetAppleCellCoord(game.apples[appleIndex].positions);

		game.applesGrid[cell.y][cell.x].insert(appleIndex);
		game.appleCellCoords[appleIndex] = cell;
	}

	static void RemoveAppleFromGrid(Game& game, int appleIndex)
	{
		const AppleCellCoord cell = game.appleCellCoords[appleIndex];

		auto rowIt = game.applesGrid.find(cell.y);
		if (rowIt == game.applesGrid.end())
		{
			return;
		}

		auto cellIt = rowIt->second.find(cell.x);
		if (cellIt == rowIt->second.end())
		{
			return;
		}

		cellIt->second.erase(appleIndex);

		if (cellIt->second.empty())
		{
			rowIt->second.erase(cellIt);
		}

		if (rowIt->second.empty())
		{
			game.applesGrid.erase(rowIt);
		}
	}

	int GetAppleCellX(Position2D position)
	{
		return static_cast<int>(position.x / APPLES_GRID_CELL_SIZE);
	}

	int GetAppleCellY(Position2D position)
	{
		return static_cast<int>(position.y / APPLES_GRID_CELL_SIZE);
	}

	AppleCellCoord GetAppleCellCoord(Position2D position)
	{
		AppleCellCoord result;
		result.x = GetAppleCellX(position);
		result.y = GetAppleCellY(position);
		return result;
	}

	void RebuildApplesGrid(Game& game)
	{
		game.applesGrid.clear();

		for (int i = 0; i < game.countApples; ++i)
		{
			AddAppleToGrid(game, i);
		}
	}

	void MoveAppleToNewPosition(Game& game, int appleIndex, Position2D newPosition)
	{
		RemoveAppleFromGrid(game, appleIndex);
		game.apples[appleIndex].positions = newPosition;
		AddAppleToGrid(game, appleIndex);
	}

	void RemoveAppleAt(Game& game, int appleIndex)
	{
		const int lastIndex = game.countApples - 1;

		RemoveAppleFromGrid(game, appleIndex);

		if (appleIndex != lastIndex)
		{
			RemoveAppleFromGrid(game, lastIndex);
			game.apples[appleIndex] = game.apples[lastIndex];
			AddAppleToGrid(game, appleIndex);
		}

		--game.countApples;
	}

	void InitApples(Apple& apples, const Game& game)
	{
		apples.sprite.setTexture(game.applesTexture);
		SetSpriteSize(apples.sprite, APPLE_SIZE, APPLE_SIZE);
		SetSpriteRelativeOrigin(apples.sprite, 0.5f, 0.5f);

		apples.positions = GetRandomPositionInScreen(SCREEN_HEIGHT, SCREEN_WIDTH);
	}

	void DrawApples(Apple& apples, sf::RenderWindow& window)
	{
		apples.sprite.setPosition(apples.positions.x, apples.positions.y);
		window.draw(apples.sprite);
	}
}