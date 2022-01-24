#pragma once
#include "Cell.hpp"
#include "GraphicElement.hpp"

class GameBoard final {
public:
	enum class game_stage { GAME, WIN, LOSING };

	GameBoard() = delete;
	explicit GameBoard(const int& size, const int& bombs_count, const int& cell_size);
	void open_cell(const sf::Vector2i& cell);
	void set_flag(const sf::Vector2i& cell);
	void draw(sf::RenderWindow& app);
	void reset_board();
	[[nodiscard]] int get_size() const;
	[[nodiscard]] bool is_clear() const;
	[[nodiscard]] int get_cell_size() const;
	[[nodiscard]] int get_cells_to_open() const;
	[[nodiscard]] int get_flags_left() const;
	[[nodiscard]] game_stage get_game_stage() const;

private:
	bool is_clear_;
	int size_;
	int cell_size_;
	game_stage stage_;
	int cells_to_open_;
	int bombs_amount_;
	int flags_remain_;
	sf::Vector2i explosion_coords_;

	std::vector<std::vector<Cell>> cells_;
	std::vector<std::vector<int>> bombs_around_cell_;

	void set_bombs(const sf::Vector2i& start_cell);
	void count_bombs_around_cell(const sf::Vector2i& cell);
	void open_empty_cell(const sf::Vector2i& cell);
	void show_all_bombs(sf::RenderWindow& app);
};
