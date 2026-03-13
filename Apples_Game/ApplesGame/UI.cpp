#include "UI.h"
#include "Constatnts.h"
#include <cassert>

namespace ApplesGame
{
	static void SetupButton(sf::RectangleShape& button, float x, float y, float width, float height)
	{
		button.setSize(sf::Vector2f(width, height));
		button.setFillColor(sf::Color::Green);
		button.setPosition(x, y);
	}

	static void SetupTextInButton(
		sf::Text& text,
		const sf::Font& font,
		const sf::String& value,
		const sf::RectangleShape& button,
		unsigned int charSize = 20)
	{
		text.setFont(font);
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::Black);
		text.setString(value);

		const sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
		text.setPosition(
			button.getPosition().x + button.getSize().x / 2.f,
			button.getPosition().y + button.getSize().y / 2.f
		);
	}

	static void SetupCenteredText(
		sf::Text& text,
		const sf::Font& font,
		const sf::String& value,
		float x,
		float y,
		unsigned int charSize,
		sf::Color color)
	{
		text.setFont(font);
		text.setCharacterSize(charSize);
		text.setFillColor(color);
		text.setString(value);

		const sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
		text.setPosition(x, y);
	}

	void InitMenu(Menu& menu)
	{
		assert(menu.font.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Black.ttf"));

		const float centerX = (SCREEN_WIDTH - menu.BUTTON_WIDTH) / 2.f;
		const float firstButtonY = 80.f;
		const float stepY = menu.BUTTON_HEIGHT + menu.GAP_BETWEEN_BUTTON;

		// ---------------------------
		// Главное меню
		// ---------------------------
		SetupCenteredText(
			menu.mainTitle,
			menu.font,
			L"МЕНЮ",
			SCREEN_WIDTH / 2.f,
			35.f,
			34,
			sf::Color::White
		);

		SetupButton(menu.menuButton1, centerX, firstButtonY + stepY * 0.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton2, centerX, firstButtonY + stepY * 1.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton3, centerX, firstButtonY + stepY * 2.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton4, centerX, firstButtonY + stepY * 3.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton5, centerX, firstButtonY + stepY * 4.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton6, centerX, firstButtonY + stepY * 5.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);


		SetupTextInButton(
			menu.textFiniteApples,
			menu.font,
			L"Конечное число яблок\n(нажмите 1 для старта)",
			menu.menuButton1
		);

		SetupTextInButton(
			menu.textInfiniteApples,
			menu.font,
			L"Бесконечные яблоки\n(нажмите 2 для старта)",
			menu.menuButton2
		);

		SetupTextInButton(
			menu.textAcceleratePlayer,
			menu.font,
			L"Ускорение игрока\n(нажмите 3 для старта)",
			menu.menuButton3
		);

		SetupTextInButton(
			menu.textAcceleratePlayerFiniteApples,
			menu.font,
			L"Ускорение + лимит яблок\n(нажмите 4 для старта)",
			menu.menuButton4
		);

		SetupTextInButton(
			menu.textScoreTable,
			menu.font,
			L"Таблица рекордов\n(нажмите 5)",
			menu.menuButton5
		);

		SetupTextInButton(
			menu.textExitGame,
			menu.font,
			L"Выйти из игры\n(нажмите Esc / 6)",
			menu.menuButton6
		);

		// ---------------------------
		// Меню паузы
		// ---------------------------
		SetupCenteredText(
			menu.pauseTitle,
			menu.font,
			L"ПАУЗА",
			SCREEN_WIDTH / 2.f,
			180.f,
			40,
			sf::Color::White
		);

		SetupButton(menu.pauseButton1, centerX, 240.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.pauseButton2, centerX, 240.f + stepY, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);

		SetupTextInButton(
			menu.textContinue,
			menu.font,
			L"Продолжить\n(нажмите 1)",
			menu.pauseButton1
		);

		SetupTextInButton(
			menu.textExitToMenu,
			menu.font,
			L"Выйти в меню\n(нажмите 2)",
			menu.pauseButton2
		);
	}

	void DrawMainMenu(Menu& menu, sf::RenderWindow& window)
	{
		window.draw(menu.mainTitle);

		window.draw(menu.menuButton1);
		window.draw(menu.menuButton2);
		window.draw(menu.menuButton3);
		window.draw(menu.menuButton4);
		window.draw(menu.menuButton5);
		window.draw(menu.menuButton6);

		window.draw(menu.textFiniteApples);
		window.draw(menu.textInfiniteApples);
		window.draw(menu.textAcceleratePlayer);
		window.draw(menu.textAcceleratePlayerFiniteApples);
		window.draw(menu.textScoreTable);
		window.draw(menu.textExitGame);

	}

	void DrawPauseMenu(Menu& menu, sf::RenderWindow& window)
	{
		sf::RectangleShape overlay(sf::Vector2f((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT));
		overlay.setFillColor(sf::Color(0, 0, 0, 180));

		window.draw(overlay);
		window.draw(menu.pauseTitle);

		window.draw(menu.pauseButton1);
		window.draw(menu.pauseButton2);

		window.draw(menu.textContinue);
		window.draw(menu.textExitToMenu);
	}
}