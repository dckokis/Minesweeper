#include "GameBoard.hpp"
#include <ctime>
#include <stack>

namespace {
	std::pair<int, int> get_coord_borders(const int cell_coord, const int size) {
		const auto min = (cell_coord == 0 ? cell_coord : cell_coord - 1);
		const auto max = (cell_coord + 1 >= size ? cell_coord : cell_coord + 1);
		return std::make_pair(min, max);
	}
}

GameBoard::GameBoard(const int& size, const int& bombs_count, const int& cell_size) :
	is_clear_(true), size_(size),
	cell_size_(cell_size),
	stage_(game_stage::GAME),
	bombs_amount_(bombs_count),
	flags_remain_(bombs_amount_)
{
	cells_to_open_ = size_ * size_ - bombs_amount_;
	explosion_coords_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
	cells_.resize(size_);
	bombs_around_cell_.resize(size_);
	for (auto i = 0; i < size_; ++i) {
		cells_[i].resize(size_);
		bombs_around_cell_[i].resize(size_);
		bombs_around_cell_[i].assign(size_, 0);
	}
}

void GameBoard::set_bombs(const sf::Vector2i& start_cell) {
	srand(time(nullptr));
	is_clear_ = false;
	for (auto i = 0; i < bombs_amount_; ++i) {
		auto new_x = rand() % size_;
		auto new_y = rand() % size_;
		while ((new_x == start_cell.x || new_x == start_cell.x + 1 || new_x == start_cell.x - 1)
			&& (new_y == start_cell.y || new_y == start_cell.y + 1 || new_y == start_cell.y - 1)
			|| cells_[new_x][new_y].is_mined()) {
			new_x = rand() % size_;
			new_y = rand() % size_;
		}
		cells_[new_x][new_y].mine_cell();
	}
	for (auto i = 0; i < GameBoard::size_; ++i) {
		for (auto j = 0; j < GameBoard::size_; ++j) {
			count_bombs_around_cell(sf::Vector2i(i, j));
		}
	}
}

void GameBoard::open_cell(const sf::Vector2i& cell) {
	if (is_clear_) {
		if (cells_[cell.x][cell.y].check_state() == Cell::cell_state::FLAG) {
			return;
		}
		set_bombs(cell);
	}
	if (!cells_[cell.x][cell.y].is_mined() && !is_clear_) {
		open_empty_cell(cell);
	}
	if (const auto check = cells_[cell.x][cell.y].try_open(); check == 1) {
		--cells_to_open_;
		if (!cells_to_open_) {
			stage_ = game_stage::WIN;
		}
	} else if (check == 0) {
		cells_to_open_ = 0;
		stage_ = game_stage::LOSING;
	}
}

void GameBoard::set_flag(const sf::Vector2i& cell) {
	if (const auto check = cells_[cell.x][cell.y].set_reset_flag(); check == 1) {
		--flags_remain_;
		if (flags_remain_ < 0) {
			cells_[cell.x][cell.y].set_reset_flag();
			++flags_remain_;
		}
	} else if (check == 0) {
		++flags_remain_;
	}
}

void GameBoard::draw(sf::RenderWindow& app) {
	sf::Sprite cell;
	app.clear(sf::Color::White);
	for (auto i = 0; i < size_; ++i) {
		for (auto j = 0; j < size_; ++j) {
			auto cell_coords = sf::Vector2i(i, j);
			const auto cell_state = cells_[i][j].check_state();
			std::string elem_name;
			if (cell_state == Cell::cell_state::CLOSE) {
				elem_name = "close";
			} else if (cell_state == Cell::cell_state::OPEN) {
				if (cells_[i][j].is_mined()) {
					if (explosion_coords_.x >= size_) {
						explosion_coords_ = cell_coords;
					} else {
						cell_coords = explosion_coords_;
					}
					elem_name = "exploded_bomb";
				} else {
					elem_name = std::to_string(bombs_around_cell_[i][j]);
				}
			} else if (cell_state == Cell::cell_state::FLAG) {
				elem_name = "flag";
			}
			GraphicElement::customize(cell, sf::Vector2i(cell_coords.x * cell_size_, cell_coords.y * cell_size_), cell_size_, elem_name);
			app.draw(cell);
		}
	}
	show_all_bombs(app);
}

void GameBoard::reset_board() {
	is_clear_ = true;
	cells_to_open_ = size_ * size_ - bombs_amount_;
	flags_remain_ = bombs_amount_;
	stage_ = game_stage::GAME;
	for (auto i = 0; i < size_; ++i) {
		bombs_around_cell_[i].assign(size_, 0);
		for (auto j = 0; j < size_; ++j) {
			cells_[i][j].clear();
		}
	}
	explosion_coords_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
}

void GameBoard::open_empty_cell(const sf::Vector2i& cell) {
	if (is_clear_ || bombs_around_cell_[cell.x][cell.y] != 0) {
		return;
	}

	std::stack<std::pair<int, int>> stack;
	stack.push(std::make_pair(cell.x, cell.y));

	while (!stack.empty()) {
		auto [new_x, new_y] = stack.top();
		stack.pop();
		if (cells_[new_x][new_y].try_open() == 1) {
			--cells_to_open_;
		}

		const auto [x_min, x_max] = get_coord_borders(new_x, size_);
		const auto [y_min, y_max] = get_coord_borders(new_y, size_);

		for (auto i = x_min; i <= x_max; ++i) {
			for (auto j = y_min; j <= y_max; ++j) {
				if (!cells_[i][j].is_mined() && cells_[i][j].check_state() == Cell::cell_state::CLOSE) {
					if (bombs_around_cell_[i][j] != 0) {
						if (cells_[i][j].try_open() == 1) {
							--cells_to_open_;
						}
					} else {
						stack.push(std::make_pair(i, j));
					}
				}
			}
		}
	}
}

void GameBoard::count_bombs_around_cell(const sf::Vector2i& cell) {
	const auto [x_min, x_max] = get_coord_borders(cell.x, size_);
	const auto [y_min, y_max] = get_coord_borders(cell.y, size_);

	for (auto i = x_min; i <= x_max; ++i) {
		for (auto j = y_min; j <= y_max; ++j) {
			if (!(i == cell.x && j == cell.y) && cells_[i][j].is_mined()) {
				++bombs_around_cell_[cell.x][cell.y];
			}
		}
	}
}

void GameBoard::show_all_bombs(sf::RenderWindow& app) {
	if (explosion_coords_.x < size_) {
		sf::Sprite bomb;
		for (auto i = 0; i < size_; ++i) {
			for (auto j = 0; j < size_; ++j) {
				const auto cell = sf::Vector2i(i, j);
				if (cells_[i][j].is_mined()
					&& !(i == explosion_coords_.x && j == explosion_coords_.y)) {
					GraphicElement::customize(bomb, sf::Vector2i(cell.x * cell_size_, cell.y * cell_size_), cell_size_, "bomb");
					open_cell(cell);
					app.draw(bomb);
				}
			}
		}
	}
}

int GameBoard::get_size() const { return size_; }

bool GameBoard::is_clear() const { return is_clear_; }

int GameBoard::get_cell_size() const { return cell_size_; }

int GameBoard::get_cells_to_open() const { return cells_to_open_; }

int GameBoard::get_flags_left() const { return flags_remain_; }

GameBoard::game_stage GameBoard::get_game_stage() const { return stage_; }