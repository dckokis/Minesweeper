#include "GameInfoBar.hpp"

namespace {
	void set_text(const sf::Font& font, sf::Text& txt, const sf::Vector2i& pos) {
		txt.setFont(font);
		txt.setStyle(sf::Text::Style::Regular);
		txt.setFillColor(sf::Color::Red);
		txt.setCharacterSize(40);
		txt.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
	}

	void set_small_window(sf::RectangleShape& small_window, const sf::Vector2i& size, const sf::Vector2i& pos) {
		small_window.setFillColor(sf::Color(230, 230, 230));
		small_window.setSize(sf::Vector2f(static_cast<float>(size.x), static_cast<float>(size.y)));
		small_window.setPosition(static_cast<float>(pos.x), static_cast<float>(pos.y));
	}

	void number_to_str(sf::Text& txt, const int& number) {
		if (number > 99) {
			txt.setString(std::to_string(number));
		}
		else if (number > 9) {
			txt.setString("0" + std::to_string(number));
		}
		else {
			txt.setString("00" + std::to_string(number));
		}
	}

	const std::string font_path = "resources/font/Montserrat-Bold.ttf";
}

GameInfoBar::GameInfoBar() {
	timer_ = GameTimer();
	small_window_size_ = sf::Vector2i(0.6 * height, 0.5 * height);
}

void GameInfoBar::draw(sf::RenderWindow& app, const GameBoard& board, const GraphicElement& graphic_element) {
	auto width = board.get_size() * board.get_cell_size();

	sf::Font font;
	if (!font.loadFromFile(font_path)) {
		throw std::invalid_argument(font_path);
	}

	constexpr int flag_counter_x = 3 * height / 4;
	const int counters_y = ((height / 4) + width);
	const int timer_x = width - (5 * height / 4);

	//windows for timer and flags amount
	sf::RectangleShape flags_window;
	set_small_window(flags_window, small_window_size_,
		sf::Vector2i(flag_counter_x, counters_y));
	app.draw(flags_window);

	sf::RectangleShape timer_window;
	set_small_window(timer_window, small_window_size_,
		sf::Vector2i(timer_x, counters_y));
	app.draw(timer_window);

	//flags left amount
	sf::Text flags_left;
	number_to_str(flags_left, board.get_flags_left());
	set_text(font, flags_left, sf::Vector2i(flag_counter_x + 7, (counters_y + 10)));
	app.draw(flags_left);

	//flag ico
	sf::Sprite flag;
	graphic_element.customize(flag, sf::Vector2i(height / 4,
		counters_y - 6), height / 2, "flag_ico");
	app.draw(flag);

	//restart btn
	sf::Sprite button;
	std::string name;
	if (board.get_game_stage() == GameBoard::game_stage::GAME || board.get_game_stage() == GameBoard::game_stage::LOSING) {
		name = "restart";
	}
	else if (board.get_game_stage() == GameBoard::game_stage::WIN) {
		name = "win";
	}
	graphic_element.customize(button, sf::Vector2i(width / 2 - height / 4 - 33, height / 4 + width - 20), height, name);
	app.draw(button);

	//timer ico
	sf::Sprite timer;
	graphic_element.customize(timer, sf::Vector2i(width - 5 * height / 8,
		counters_y), height / 2, "clock");
	app.draw(timer);

	//timer
	sf::Text time;
	if (board.get_cells_to_open()) {
		if (board.is_empty()) {
			timer_.restart();
		}
	}
	number_to_str(time,timer_.get_time());
	set_text(font, time, sf::Vector2i(timer_x + 3, (counters_y + 10)));
	app.draw(time);

	//back btn
	sf::Sprite back_btn;
	graphic_element.customize(back_btn, sf::Vector2i(0, width + 100), height / 3, "back");
	app.draw(back_btn);
}

int GameInfoBar::get_height() {
	return height;
}

bool GameInfoBar::restart_click_registered(const sf::Vector2i& point, const GameBoard& board) {
	const auto board_width = board.get_size() * board.get_cell_size();
	const sf::IntRect area(board_width / 2 - height / 2,
	                       board_width + height / 4 - 10, height - 35, height - 10);
	return area.contains(point);
}

bool GameInfoBar::back_click_registered(const sf::Vector2i& point, const GameBoard& board) {
	const sf::IntRect area(0, board.get_size() * board.get_cell_size() + 105, height / 3, height / 3);
	return area.contains(point);
}