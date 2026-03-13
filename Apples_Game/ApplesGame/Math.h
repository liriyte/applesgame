#pragma once

namespace sf 
{
	class Sprite;
}
namespace ApplesGame
{
	struct Vector2D
	{
		float x = 0;
		float y = 0;
	};
	typedef Vector2D Position2D;

	Position2D GetRandomPositionInScreen(float screenHeight, float screenWidth);

	bool IsRectengleCollide
	(Position2D rect1Poxition, Vector2D rect1Size,
		Position2D rect2Position, Vector2D rect2Size);

	bool IsApplesCollide(Position2D rectCenter, float rectHalfSize,
		Position2D circleCenter, float circleRadius);

	void SetSpriteSize(sf::Sprite& sprite, float desiredWidth, float desiredHeight);

	void SetSpriteRelativeOrigin(sf::Sprite& sprite, float originX, float originY);

	// функция для генерации рандомного числа яблок
	int GetRansomApplesCount(int minNumber, int maxNumber);
}

