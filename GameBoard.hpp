#pragma once
#include <SFML/Graphics.hpp>
#include "Cell.hpp"
#include "GraphicElement.hpp"

class GameBoard final {
public:
	GameBoard() = delete;
	explicit GameBoard(const int& size, const int& bombs_count, const int& cell_size);

	void open_cell(const sf::Vector2i& v);
	void flag(const sf::Vector2i& v);
	void draw(sf::RenderWindow& app, const GraphicElement& img);

	enum class board_condition { in_process, win, losing };

	void clear();

	int size() const;
	bool empty() const;
	int cell_size() const;
	int need_to_open() const;
	int flags_count() const;
	board_condition condition() const;

private:
	bool empty_;
	int size_;
	int cell_size_;
	board_condition condition_;

	int need_to_open_;
	int bombs_count_;
	int flags_count_;
	sf::Vector2i bang_vector_;

	void set_bombs(const sf::Vector2i& v);
	void count_bombs_around(const sf::Vector2i& v);
	void open_empty_cell(const sf::Vector2i& v);

	std::vector<std::vector<Cell>> cells_;
	std::vector<std::vector<int>> bombs_around_;
};
