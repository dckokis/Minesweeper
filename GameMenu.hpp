#pragma once
#include "Game.hpp"

class GameMenu final {
public:
	GameMenu() = delete;

	class MenuButton final {
	public:
		MenuButton() = delete;
		explicit MenuButton(const sf::Vector2i& size, const std::string& txt, const sf::Vector2i& pos, const sf::Font& font);
		void draw(sf::RenderWindow& app) const;
		bool click_registered(const sf::Vector2i& point) const;
	private:
		sf::IntRect borders_;
		sf::Text button_text_;
	};

	static bool start();
	static Game::level get_level();

private:
	constexpr static inline int size = 600;
	static inline Game::level level_ = Game::level::LIGHT;
};