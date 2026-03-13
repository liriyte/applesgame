#include "Math.h"
#include <cstdlib>
#include <algorithm>
#include <SFML/Graphics.hpp>

namespace ApplesGame
{
	Position2D GetRandomPositionInScreen(float screenHeight, float screenWidth)
	{
		Position2D result;
		result.x = rand() / (float)RAND_MAX * screenWidth;
		result.y = rand() / (float)RAND_MAX * screenHeight;
		return result;
	}

	bool IsRectengleCollide(Position2D rect1Poxition, Vector2D rect1Size, Position2D rect2Position, Vector2D rect2Size)
	{
		float dx = (float)fabs(rect1Poxition.x - rect2Position.x);
		float dy = (float)fabs(rect1Poxition.y - rect2Position.y);
		return (dx <= (rect1Size.x + rect2Size.x) / 2.f &&
			dy <= (rect1Size.y + rect2Size.y) / 2.f);
	}

	bool IsApplesCollide(Position2D rectCenter, float rectHalfSize, Position2D circleCenter, float circleRadius)
	{
		float closestX = std::max(rectCenter.x - rectHalfSize,
			std::min(circleCenter.x, rectCenter.x + rectHalfSize));
		float closestY = std::max(rectCenter.y - rectHalfSize,
			std::min(circleCenter.y, rectCenter.y + rectHalfSize));

		float dx = closestX - circleCenter.x;
		float dy = closestY - circleCenter.y;

		return (dx * dx + dy * dy) <= (circleRadius * circleRadius);
	}

	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sf::Vector2f scale = { desiredWidth / spriteRect.width, desiredHeight / spriteRect.height };
		sprite.setScale(scale);
	}

	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY)
	{
		sf::FloatRect spriteRect = sprite.getLocalBounds();
		sprite.setOrigin(originX * spriteRect.width, originY * spriteRect.height);
	}

	int GetRansomApplesCount(int minNumber, int maxNumber)
	{
		return minNumber + std::rand() % (maxNumber - minNumber +1);
	}

}


