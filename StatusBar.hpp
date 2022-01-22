#pragma once
#include "GameBoard.hpp"
#include "FontManager.hpp"
#include <SFML/Graphics.hpp>

class StatusBar final {
public:
	StatusBar();

	void draw(sf::RenderWindow& app, const GameBoard& board, const GraphicElement& graphic_element);

	constexpr static inline int height = 148;
	static bool is_smile_area_contains(const sf::Vector2i& v, const GameBoard& board);
	static bool is_back_area_contains(const sf::Vector2i& v, const GameBoard& board);
private:
	sf::Font font_;
	sf::Clock clock_;
	int i_time_;

	void set_text(sf::Text& txt, const sf::Vector2i& pos);
	static void set_rect(sf::RectangleShape& rect, const sf::Vector2i& size, const sf::Vector2i& pos);
	static void format_numbers(sf::Text& txt, const int& number);
};