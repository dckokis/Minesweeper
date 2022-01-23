#include "Game.hpp"

bool Game::run(const Game::level level) {
	int size, bombs_count, cell_size;
	set_game_params(level, size, bombs_count, cell_size);
	auto board = GameBoard(size, bombs_count, cell_size);
	auto board_wight = board.get_size() * board.get_cell_size();
	sf::RenderWindow app(sf::VideoMode(board_wight,
	                                   board_wight + GameInfoBar::get_height()), "Minesweeper", sf::Style::Close);
	GraphicElement graphic_element;
	while (new_game_) {
		board.clear();
		GameInfoBar info_bar;
		while (app.isOpen()) {
			new_game_ = 0;
			mouse_click_process(app, board);
			if (!new_game_) {
				board.draw(app, graphic_element);
				info_bar.draw(app, board, graphic_element);
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

void Game::set_game_params(const level& level, int& size, int& bombs_count, int& cell_size) {
	if (level == level::LIGHT) {
		cell_size = 60;
		size = 10;
		bombs_count = 10;
	}
	else if (level == level::MEDIUM) {
		cell_size = 45;
		size = 15;
		bombs_count = 30;
	}
	else {
		cell_size = 32;
		size = 25;
		bombs_count = 90;
	}
}

void Game::mouse_click_process(sf::RenderWindow& app, GameBoard& board) {
	const auto pos = sf::Mouse::getPosition(app);
	const auto cell = sf::Vector2i(pos.x / board.get_cell_size(), pos.y / board.get_cell_size());

	sf::Event event{};
	while (app.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			app.close();
		}
		if (event.type == sf::Event::MouseButtonPressed && cell.y < board.get_size() && board.get_cells_to_open()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				board.open_cell(cell);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				board.set_flag(cell);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& GameInfoBar::restart_click_registered(sf::Mouse::getPosition(app), board)) {
			new_game_ = 1;
			return;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& GameInfoBar::back_click_registered(sf::Mouse::getPosition(app), board)) {
			new_game_ = -1;
			return;
		}
	}
}

void Game::game_over(sf::RenderWindow& app, const GameBoard& board) {
	sf::Event event{};
	if (!board.get_cells_to_open()) {
		while (app.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				app.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& GameInfoBar::restart_click_registered(sf::Mouse::getPosition(app), board)) {
				new_game_ = 1;
				return;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& GameInfoBar::back_click_registered(sf::Mouse::getPosition(app), board)) {
				new_game_ = -1;
				return;
			}
		}
	}
}