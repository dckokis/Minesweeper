#include "GameMenu.hpp"

namespace {
	void draw_title(const std::string& title, sf::RenderWindow& app, const sf::Font& font, const int size) {
		sf::Text lvl_select;
		lvl_select.setFont(font);
		lvl_select.setString(title);
		lvl_select.setPosition(static_cast<float>(size / 4) - 118, static_cast<float>(size / 24));
		lvl_select.setStyle(sf::Text::Style::Regular);
		lvl_select.setFillColor(sf::Color::Red);
		lvl_select.setCharacterSize(50);
		app.draw(lvl_select);
	}

	const std::string font_path = "resources/font/Montserrat-Bold.ttf";
}

bool GameMenu::start() {
	sf::RenderWindow app(sf::VideoMode(size, size), "Main Menu", sf::Style::Close);
	sf::Font font;
	if (!font.loadFromFile(font_path)) {
		throw MNSWPexceptions("unable to find font by path: " + font_path);
	}

	while (app.isOpen()) {
		auto light_level_btn = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
			"LIGHT", sf::Vector2i(size / 4, size / 4), font);
		auto medium_level_btn = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
			"MEDIUM", sf::Vector2i(size / 4, size / 2), font);
		auto hard_btn = GameMenu::MenuButton(sf::Vector2i(size / 2, size / 6),
			"HARD", sf::Vector2i(size / 4, 3 * size / 4), font);
		sf::Event event{};
		app.clear(sf::Color(150, 150, 150));
		while (app.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				app.close();
				return false;
			}
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (light_level_btn.click_registered(sf::Mouse::getPosition(app))) {
					level_ = Game::level::LIGHT;
				} else if (medium_level_btn.click_registered(sf::Mouse::getPosition(app))) {
					level_ = Game::level::MEDIUM;
				} else if (hard_btn.click_registered(sf::Mouse::getPosition(app))) {
					level_ = Game::level::HARD;
				}
				app.close();
			}
		}
		light_level_btn.draw(app);
		medium_level_btn.draw(app);
		hard_btn.draw(app);
		draw_title("SELECT GAME LEVEL", app, font, size);
		app.display();
	}
	return true;
}

GameMenu::MenuButton::MenuButton(const sf::Vector2i& size, const std::string& txt, const sf::Vector2i& pos, const sf::Font& font) {
	borders_.left = pos.x;
	borders_.top = pos.y;
	borders_.width = size.x;
	borders_.height = size.y;

	button_text_.setString(txt);
	button_text_.setFont(font);
	button_text_.setStyle(sf::Text::Style::Regular);
	if (txt == "LIGHT") {
		button_text_.setFillColor(sf::Color::Green);
		button_text_.setPosition(75 + static_cast<float>(borders_.left), 15 + static_cast<float>(borders_.top));
	} else if (txt == "MEDIUM") {
		button_text_.setPosition(37 + static_cast<float>(borders_.left), 15 + static_cast<float>(borders_.top));
		button_text_.setFillColor(sf::Color::White);
	} else {
		button_text_.setFillColor(sf::Color::Red);
		button_text_.setPosition(75 + static_cast<float>(borders_.left), 15 + static_cast<float>(borders_.top));
	}
	button_text_.setCharacterSize(50);
}

void GameMenu::MenuButton::draw(sf::RenderWindow& app) const {
	sf::RectangleShape rect;
	rect.setFillColor(sf::Color(90, 90, 90));
	rect.setPosition(static_cast<float>(borders_.left), static_cast<float>(borders_.top));
	rect.setSize(sf::Vector2f(static_cast<float>(borders_.width), static_cast<float>(borders_.height)));
	app.draw(rect);
	app.draw(button_text_);
}

Game::level GameMenu::get_level() { return level_; }

bool GameMenu::MenuButton::click_registered(const sf::Vector2i& point) const { return borders_.contains(point); }