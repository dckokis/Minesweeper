#include "GameBoard.hpp"
#include <cstdlib>
#include <ctime>
#include <stack>

GameBoard::GameBoard(const int& size, const int& bombs_count, const int& cell_size) :
	empty_(true), size_(size),
	cell_size_(cell_size),
	condition_(board_condition::in_process),
	bombs_count_(bombs_count),
	flags_count_(bombs_count_)
{
	need_to_open_ = size_ * size_ - bombs_count_;
	bang_vector_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
	cells_.resize(size_);
	bombs_around_.resize(size_);
	for (auto i = 0; i < size_; ++i) {
		cells_[i].resize(size_);
		bombs_around_[i].resize(size_);
		bombs_around_[i].assign(size_, 0);
	}
}

void GameBoard::set_bombs(const sf::Vector2i& v) {
	srand(time(nullptr));
	empty_ = false;
	for (auto i = 0; i < bombs_count_; ++i) {
		auto new_x = rand() % size_;
		auto new_y = rand() % size_;
		while ((new_x == v.x || new_x == v.x + 1 || new_x == v.x - 1)
			&& (new_y == v.y || new_y == v.y + 1 || new_y == v.y - 1)
			|| cells_[new_x][new_y].is_mined()) {
			new_x = rand() % size_;
			new_y = rand() % size_;
		}
		cells_[new_x][new_y].set_bomb();
	}
	for (auto i = 0; i < GameBoard::size_; ++i) {
		for (auto j = 0; j < GameBoard::size_; ++j) {
			count_bombs_around(sf::Vector2i(i, j));
		}
	}
}

void GameBoard::open_cell(const sf::Vector2i& v) {
	if (empty_ && cells_[v.x][v.y].condition() == Cell::cellState::flag) {
		return;
	}
	else if (empty_) {
		set_bombs(v);
	}
	if (!cells_[v.x][v.y].is_mined() && !empty_) {
		open_empty_cell(v);
	}
	const auto check = cells_[v.x][v.y].open();
	if (check == 1) {
		--need_to_open_;
		if (!need_to_open_) {
			condition_ = board_condition::win;
		}
	}
	else if (check == 0) {
		need_to_open_ = 0;
		condition_ = board_condition::losing;
	}
}

void GameBoard::flag(const sf::Vector2i& v) {
	if (const auto check = cells_[v.x][v.y].flag(); check == 1) {
		--flags_count_;
		if (flags_count_ < 0) {
			cells_[v.x][v.y].flag();
			++flags_count_;
		}
	}
	else if (check == 0) {
		++flags_count_;
	}
}

void GameBoard::draw(sf::RenderWindow& app, const GraphicElement& img) {
	sf::Sprite s;
	app.clear(sf::Color(219, 219, 219));

	for (auto i = 0; i < size_; ++i) {
		for (auto j = 0; j < size_; ++j) {
			auto v = sf::Vector2i(i, j);
			const auto condition = cells_[i][j].condition();
			std::string str;

			if (condition == Cell::cellState::close) {
				str = "close";
			}
			else if (condition == Cell::cellState::open) {
				if (cells_[i][j].is_mined()) {
					if (bang_vector_.x >= size_) {
						bang_vector_ = v;
					}
					else {
						v = bang_vector_;
					}
					str = "red_bomb";
				}
				else {
					str = std::to_string(bombs_around_[i][j]);
				}
			}
			else if (condition == Cell::cellState::flag) {
				str = "flag";
			}
			img.customize(s, sf::Vector2i(v.x * cell_size_, v.y * cell_size_), cell_size_, str);
			app.draw(s);
		}
	}
	if (bang_vector_.x < size_) {
		for (auto i = 0; i < size_; ++i) {
			for (auto j = 0; j < size_; ++j) {
				const auto v = sf::Vector2i(i, j);
				if (cells_[i][j].is_mined()
					&& !(i == bang_vector_.x && j == bang_vector_.y)) {
					img.customize(s, sf::Vector2i(v.x * cell_size_, v.y * cell_size_), cell_size_, "bomb");
					open_cell(v);
					app.draw(s);
				}
			}
		}
	}
}

void GameBoard::clear() {
	empty_ = true;
	need_to_open_ = size_ * size_ - bombs_count_;
	flags_count_ = bombs_count_;
	condition_ = board_condition::in_process;
	for (auto i = 0; i < size_; ++i) {
		bombs_around_[i].assign(size_, 0);
		for (auto j = 0; j < size_; ++j) {
			cells_[i][j].clear();
		}
	}
	bang_vector_ = { std::numeric_limits<int>::max(), std::numeric_limits<int>::max() };
}

int GameBoard::size() const {
	return size_;
}

bool GameBoard::empty() const {
	return empty_;
}

int GameBoard::cell_size() const {
	return cell_size_;
}

int GameBoard::need_to_open() const {
	return need_to_open_;
}

int GameBoard::flags_count() const {
	return flags_count_;
}

GameBoard::board_condition GameBoard::condition() const {
	return condition_;
}

void GameBoard::open_empty_cell(const sf::Vector2i& v) {
	if (empty_ || bombs_around_[v.x][v.y] != 0) {
		return;
	}

	std::stack<std::pair<int, int>> stack;
	stack.push(std::make_pair(v.x, v.y));

	while (!stack.empty()) {
		auto [new_x, new_y] = stack.top();
		stack.pop();
		if (cells_[new_x][new_y].open() == 1) {
			--need_to_open_;
		}

		const auto x_min = (new_x == 0 ? new_x : new_x - 1);
		const auto x_max = (new_x + 1 >= size_ ? new_x : new_x + 1);
		const auto y_min = (new_y == 0 ? new_y : new_y - 1);
		const auto y_max = (new_y + 1 >= size_ ? new_y : new_y + 1);

		for (auto i = x_min; i <= x_max; ++i) {
			for (auto j = y_min; j <= y_max; ++j) {
				if (bombs_around_[i][j] != 0 && !cells_[i][j].is_mined()
					&& cells_[i][j].condition() == Cell::cellState::close) {
					if (cells_[i][j].open() == 1) {
						--need_to_open_;
					}
				}
				if (!cells_[new_x][new_y].is_mined() && bombs_around_[i][j] == 0
					&& cells_[i][j].condition() == Cell::cellState::close) {
					stack.push(std::make_pair(i, j));
				}
			}
		}
	}
}

void GameBoard::count_bombs_around(const sf::Vector2i& v) {
	const auto x_min = (v.x == 0 ? v.x : v.x - 1);
	const auto x_max = (v.x + 1 >= size_ ? v.x : v.x + 1);
	const auto y_min = (v.y == 0 ? v.y : v.y - 1);
	const auto y_max = (v.y + 1 >= size_ ? v.y : v.y + 1);

	for (auto i = x_min; i <= x_max; ++i) {
		for (auto j = y_min; j <= y_max; ++j) {
			if (!(i == v.x && j == v.y) && cells_[i][j].is_mined()) {
				++bombs_around_[v.x][v.y];
			}
		}
	}
}