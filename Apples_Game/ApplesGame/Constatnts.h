#pragma once
#include <string>

namespace ApplesGame
{
	const std::string RESOURCES_PATH = "Resources/";
	const int SCREEN_WIDTH = 800;
	const int SCREEN_HEIGHT = 600;
	const float INITIAL_SPEED = 100.f;
	const float PLAYER_SIZE = 20.f;
	const float ACCELERATION = 20.f;
	const float APPLE_SIZE = 20.f;
	const int NUM_ROCKS = 8;
	const float ROCKS_SIZE = 30.f;
	const float PAUSE_TIME = 4.f;

	const int MIN_NUM_APPLES = 10;
	const int MAX_NUM_APPLES = 30;

	const int MIN_RAND_SCORE = 1;
	const int MAX_RAND_SCORE = 75;
	const int MAX_SCORES = 12; // КОНСТАНТЫ ТАБЛИЦЫ РЕКОРДОВ
	const int MAX_ROWS_SHOWN = 6;  // КОНСТАНТЫ ТАБЛИЦЫ РЕКОРДОВ

	const float APPLES_GRID_CELL_SIZE = APPLE_SIZE; // размер клетки = размер яблока

}


