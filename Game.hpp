#pragma once
#include "GameBoard.hpp"
#include "GameInfoBar.hpp"

class Game final {
public:
	enum class level { LIGHT, MEDIUM, HARD };

	Game() : new_game_(1) {}
	bool run(const Game::level level);
	void set_new_game(int value);

private:
	int new_game_;

	struct LevelOptions {
		int cell_size;
		int size;
		int bombs_count;
	};

	void mouse_click_process(sf::RenderWindow& app, GameBoard& board);
	void game_over(sf::RenderWindow& app, const GameBoard& board);
	void set_game_params(const level& level, int& size, int& bombs_count, int& cell_size) const;
	LevelOptions light_level_params_ = { 60, 10, 10 };
	LevelOptions medium_level_params_ = { 45, 15, 30 };
	LevelOptions hard_level_params_ = { 32, 25, 90 };
	std::unordered_map<level, LevelOptions> level_params_ = { {level::LIGHT, light_level_params_}, {level::MEDIUM, medium_level_params_}, {level::HARD, hard_level_params_} };
};