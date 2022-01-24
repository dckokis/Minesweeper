#include "Game.hpp"

namespace {
	int game_bar_mouse_click_process(sf::RenderWindow& app, const GameBoard& board, const sf::Event& event) {
		if (event.type == sf::Event::Closed) {
			app.close();
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& GameInfoBar::restart_click_registered(sf::Mouse::getPosition(app), board)) {
			return 1;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& GameInfoBar::back_click_registered(sf::Mouse::getPosition(app), board)) {
			return -1;
		}
		return 0;
	}
}

bool Game::run(const Game::level level) {
	int size, bombs_count, cell_size;
	set_game_params(level, size, bombs_count, cell_size);
	auto board = GameBoard(size, bombs_count, cell_size);
	auto board_wight = board.get_size() * board.get_cell_size();
	sf::RenderWindow app(sf::VideoMode(board_wight,
	                                   board_wight + GameInfoBar::get_height()), "Minesweeper", sf::Style::Close);
	GraphicElement::load_textures();
	while (new_game_) {
		board.reset_board();
		GameInfoBar info_bar;
		while (app.isOpen()) {
			mouse_click_process(app, board);
			if (!new_game_) {
				board.draw(app);
				info_bar.draw(app, board);
				app.display();
				game_over(app, board);
			}
			if (new_game_ == 1) {
				break;
			}
			if (new_game_ == -1) {
				app.close();
				return true;
			}
		}
	}
	return false;
}

void Game::set_game_params(const level& level, int& size, int& bombs_count, int& cell_size) const {
	switch (level) {
	case level::LIGHT:
		cell_size = level_params_.at(level::LIGHT).cell_size;
		size = level_params_.at(level::LIGHT).size;
		bombs_count = level_params_.at(level::LIGHT).bombs_count;
		break;
	case level::MEDIUM:
		cell_size = level_params_.at(level::MEDIUM).cell_size;
		size = level_params_.at(level::MEDIUM).size;
		bombs_count = level_params_.at(level::MEDIUM).bombs_count;
		break;
	case level::HARD:
		cell_size = level_params_.at(level::HARD).cell_size;
		size = level_params_.at(level::HARD).size;
		bombs_count = level_params_.at(level::HARD).bombs_count;
		break;
	}
}

void Game::mouse_click_process(sf::RenderWindow& app, GameBoard& board) {
	const auto pos = sf::Mouse::getPosition(app);
	const auto cell = sf::Vector2i(pos.x / board.get_cell_size(), pos.y / board.get_cell_size());

	sf::Event event{};
	while (app.pollEvent(event)) {
		if (event.type == sf::Event::MouseButtonPressed && cell.y < board.get_size() && board.get_cells_to_open()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				board.open_cell(cell);
			} else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				board.set_flag(cell);
			}
		}
		set_new_game(game_bar_mouse_click_process(app, board, event));
	}
}

void Game::game_over(sf::RenderWindow& app, const GameBoard& board) {
	sf::Event event{};
	if (!board.get_cells_to_open()) {
		while (app.pollEvent(event)) {
			set_new_game(game_bar_mouse_click_process(app, board, event));
		}
	}
}

void Game::set_new_game(const int value) { new_game_ = value; }