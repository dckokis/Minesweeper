#include "GameMenu.hpp"
#include <iostream>

int main() {
	try {
		while (GameMenu::start()) {
			Game game;
			if (!game.run(GameMenu::get_level())) {
				break;
			}
		}
	}
	catch (std::exception& exception) {
		std::cerr << exception.what() << std::endl;
	}
	return 0;
}