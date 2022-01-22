#include "Game.hpp"

bool Game::run(const Game::level level) {
	int size, bombs_count, cell_size;
	set_field_parameters(level, size, bombs_count, cell_size);
	auto board = GameBoard(size, bombs_count, cell_size);

	sf::RenderWindow app(sf::VideoMode(board.size() * board.cell_size(),
	                                   board.size() * board.cell_size() + StatusBar::height), "Minesweeper", sf::Style::Close);

	while (new_game_) {
		board.clear();
		StatusBar sb;
		while (app.isOpen()) {
			new_game_ = 0;
			event_processing(app, board);
			if (!new_game_) {
				GraphicElement graphic_element;
				board.draw(app, graphic_element);
				sb.draw(app, board, graphic_element);
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

void Game::set_field_parameters(const level& level, int& size, int& bombs_count, int& cell_size) {
	if (level == level::beginner) {
		cell_size = 60;
		size = 9; bombs_count = 10;
	}
	else if (level == level::intermediate) {
		cell_size = 45;
		size = 16; bombs_count = 40;
	}
	else {
		cell_size = 32;
		size = 23; bombs_count = 90;
	}
}

void Game::event_processing(sf::RenderWindow& app, GameBoard& board) {
	const auto pos = sf::Mouse::getPosition(app);
	const auto v = sf::Vector2i(pos.x / board.cell_size(), pos.y / board.cell_size());

	sf::Event e{};
	while (app.pollEvent(e)) {
		if (e.type == sf::Event::Closed) {
			app.close();
		}
		if (e.type == sf::Event::MouseButtonPressed && v.y < board.size() && board.need_to_open()) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				board.open_cell(v);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
				board.flag(v);
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& StatusBar::is_smile_area_contains(sf::Mouse::getPosition(app), board)) {
			new_game_ = 1;
			return;
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
			&& StatusBar::is_back_area_contains(sf::Mouse::getPosition(app), board)) {
			new_game_ = -1;
			return;
		}
	}
}

void Game::game_over(sf::RenderWindow& app, const GameBoard& board) {
	sf::Event e{};
	if (!board.need_to_open()) {
		while (app.waitEvent(e)) {
			if (e.type == sf::Event::Closed) {
				app.close();
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& StatusBar::is_smile_area_contains(sf::Mouse::getPosition(app), board)) {
				new_game_ = 1;
				return;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& StatusBar::is_back_area_contains(sf::Mouse::getPosition(app), board)) {
				new_game_ = -1;
				return;
			}
		}
	}
}