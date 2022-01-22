#pragma once
#include "GameBoard.hpp"
#include "StatusBar.hpp"

class Game final {
public:
	enum class level { LIGHT, MEDIUM, HARD };

	Game() : new_game_(1) {}
	bool run(const Game::level level);

private:
	int new_game_;

	void event_processing(sf::RenderWindow& app, GameBoard& board);
	void game_over(sf::RenderWindow& app, const GameBoard& board);
	static void set_field_parameters(const level& level, int& size, int& bombs_count, int& cell_size);
};