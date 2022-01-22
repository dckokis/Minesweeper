#include "GameMenu.hpp"

bool GameMenu::start() {
	sf::RenderWindow app(sf::VideoMode(size, size), "Minesweeper Menu", sf::Style::Close);
	sf::Font font;
	if (!font.loadFromFile(font_path)) {
		throw std::invalid_argument(font_path);
	}

	auto beginner = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
		"BEGINNER", sf::Vector2i(size / 4, size / 4), font);
	auto intermediate = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
		"INTERMEDIATE", sf::Vector2i(size / 4, size / 2), font);
	auto expert = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
		"EXPERT", sf::Vector2i(size / 4, 3 * size / 4), font);

	while (app.isOpen()) {
		sf::Event e{};
		app.clear(sf::Color(219, 219, 219));
		while (app.pollEvent(e)) {

			if (e.type == sf::Event::Closed) {
				app.close();
				return false;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& beginner.contains(sf::Mouse::getPosition(app))) {
				level_ = Game::level::beginner;
				app.close();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& intermediate.contains(sf::Mouse::getPosition(app))) {
				level_ = Game::level::intermediate;
				app.close();
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)
				&& expert.contains(sf::Mouse::getPosition(app))) {
				level_ = Game::level::expert;
				app.close();
			}
		}
		beginner.draw(app);
		intermediate.draw(app);
		expert.draw(app);
		draw_information(app);

		sf::Text choose_level;
		choose_level.setFont(font);
		choose_level.setString("CHOOSE THE LEVEL");
		choose_level.setPosition(static_cast<float>(size / 4) - 10, static_cast<float>(size / 24));
		choose_level.setStyle(sf::Text::Style::Bold);
		choose_level.setFillColor(sf::Color::Red);
		choose_level.setCharacterSize(80);
		app.draw(choose_level);

		app.display();
	}
	return true;
}

 Game::level GameMenu::get_level() {
	return level_;
}

void GameMenu::draw_information(sf::RenderWindow& app) {
	const auto texture = new sf::Texture;
	if (!texture->loadFromFile("images/menu.png")) {
		throw std::invalid_argument("images/menu.png");
	}
	sf::Sprite s;
	s.setTexture(*texture);
	s.setPosition(0, size / 4);
	app.draw(s);
	delete texture;
}

GameMenu::MenuButton::MenuButton(const sf::Vector2i& size, const std::string& txt, const sf::Vector2i& pos, const sf::Font& font) {
	borders_.left = pos.x;
	borders_.top = pos.y;
	borders_.width = size.x;
	borders_.height = size.y;

	button_text_.setString(txt);
	button_text_.setFont(font);
	button_text_.setPosition(10 + static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	button_text_.setStyle(sf::Text::Style::Bold);
	button_text_.setFillColor(sf::Color::White);
	button_text_.setCharacterSize(80);
}

void GameMenu::MenuButton::draw(sf::RenderWindow& app) const {
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	rect.setSize(sf::Vector2f(static_cast<float>(borders_.width), static_cast<float>(borders_.height)));
	app.draw(rect);
	app.draw(button_text_);
}

bool GameMenu::MenuButton::contains(const sf::Vector2i& v) const {
	return borders_.contains(v);
}