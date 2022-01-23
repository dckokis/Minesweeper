#pragma once
#include "GameBoard.hpp"
#include <SFML/Graphics.hpp>
#include "GameTimer.hpp"


class GameInfoBar final {
public:
	GameInfoBar();
	void draw(sf::RenderWindow& app, const GameBoard& board, const GraphicElement& graphic_element);
	static int get_height();
	static bool restart_click_registered(const sf::Vector2i& point, const GameBoard& board);
	static bool back_click_registered(const sf::Vector2i& point, const GameBoard& board);
private:
	constexpr static inline int height = 145;
	GameTimer timer_;
	sf::Vector2i small_window_size_;
};