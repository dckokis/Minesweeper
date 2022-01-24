#pragma once
#include "GameBoard.hpp"
#include "GameTimer.hpp"
#include "MNSWPexceptions.hpp"

class GameInfoBar final {
public:
	GameInfoBar();
	void draw(sf::RenderWindow& app, const GameBoard& board);
	static int get_height();
	static bool restart_click_registered(const sf::Vector2i& point, const GameBoard& board);
	static bool back_click_registered(const sf::Vector2i& point, const GameBoard& board);

private:
	constexpr static inline int height = 145;
	GameTimer timer_;
	sf::Vector2i small_window_size_;
	static void draw_back_btn(sf::RenderWindow& app, const sf::Vector2i& pos);
	static void draw_restart_btn(sf::RenderWindow& app, const GameBoard& board, const sf::Vector2i& pos);
	void draw_small_windows(sf::RenderWindow& app, const sf::Vector2i& flag_wind_pos, const sf::Vector2i& timer_wind_pos) const;
	static void print_counter(sf::RenderWindow& app, const sf::Vector2i& pos, const sf::Font& font, sf::Text& txt, const int& counter);
	static void draw_icos(sf::RenderWindow& app, const sf::Vector2i& flag_ico_pos, const sf::Vector2i& timer_ico_pos);
};
