#include "UI.h"
#include "Constatnts.h"
#include "Math.h"
#include <cassert>

namespace ApplesGame
{
	// устанавливаю текстуру на спрайт кнопки
	static void SetupButton(
		sf::Sprite& button,
		const sf::Texture& texture,
		float x,
		float y,
		float width,
		float height)
	{
		button.setTexture(texture);
		SetSpriteSize(button, width, height);
		button.setPosition(x, y);
	}

	static void SetupTextInButton(
		sf::Text& text,
		const sf::Font& font,
		const sf::String& value,
		const sf::Sprite& button,
		unsigned int charSize = 15)
	{
		text.setFont(font);
		text.setCharacterSize(charSize);
		text.setFillColor(sf::Color::White);
		text.setString(value);

		const sf::FloatRect textBounds = text.getLocalBounds();
		text.setOrigin(
			textBounds.left + textBounds.width / 2.f,
			textBounds.top + textBounds.height / 2.f
		);

		const sf::FloatRect buttonBounds = button.getGlobalBounds();
		text.setPosition(
			buttonBounds.left + buttonBounds.width / 2.f,
			buttonBounds.top + buttonBounds.height / 2.f
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
		assert(menu.buttonTexture.loadFromFile(RESOURCES_PATH + "\\Button.png"));

		const float centerX = (SCREEN_WIDTH - menu.BUTTON_WIDTH) / 2.f;
		const float firstButtonY = 80.f;
		const float stepY = menu.BUTTON_HEIGHT + menu.GAP_BETWEEN_BUTTON;

		// Главное меню
		SetupCenteredText(
			menu.mainTitle,
			menu.font,
			L"МЕНЮ",
			SCREEN_WIDTH / 2.f,
			55.f,
			34,
			sf::Color::White
		);

		SetupButton(menu.menuButton1, menu.buttonTexture, centerX, firstButtonY + stepY * 0.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton2, menu.buttonTexture, centerX, firstButtonY + stepY * 1.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton3, menu.buttonTexture, centerX, firstButtonY + stepY * 2.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton4, menu.buttonTexture, centerX, firstButtonY + stepY * 3.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton5, menu.buttonTexture, centerX, firstButtonY + stepY * 4.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.menuButton6, menu.buttonTexture, centerX, firstButtonY + stepY * 5.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);

		SetupTextInButton(
			menu.textFiniteApples,
			menu.font,
			L"Лимит яблок\n([1] для старта)",
			menu.menuButton1
		);

		SetupTextInButton(
			menu.textInfiniteApples,
			menu.font,
			L"Без лимита яблок\n([2] для старта)",
			menu.menuButton2
		);

		SetupTextInButton(
			menu.textAcceleratePlayer,
			menu.font,
			L"Ускорение игрока\n([3] для старта)",
			menu.menuButton3
		);

		SetupTextInButton(
			menu.textAcceleratePlayerFiniteApples,
			menu.font,
			L"Ускорение + лимит яблок\n([4] для старта)",
			menu.menuButton4
		);

		SetupTextInButton(
			menu.textScoreTable,
			menu.font,
			L"Таблица рекордов\n(нажмите [5])",
			menu.menuButton5
		);

		SetupTextInButton(
			menu.textExitGame,
			menu.font,
			L"Выйти из игры\n(нажмите [Esc]/[6])",
			menu.menuButton6
		);

		// Меню паузы
		SetupCenteredText(
			menu.pauseTitle,
			menu.font,
			L"ПАУЗА",
			SCREEN_WIDTH / 2.f,
			180.f,
			40,
			sf::Color::White
		);

		SetupButton(menu.pauseButton1, menu.buttonTexture, centerX, 240.f, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);
		SetupButton(menu.pauseButton2, menu.buttonTexture, centerX, 240.f + stepY, menu.BUTTON_WIDTH, menu.BUTTON_HEIGHT);

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