#include "GameMenu.hpp"
#include <iostream>

int main() {
	try {
		while (GameMenu::start()) {
			Game g;
			if (!g.run(GameMenu::get_level())) {
				break;
			}
		}
	}
	catch (std::invalid_argument& arg) {
		std::cerr << arg.what() << std::endl;
		std::cerr << "file does not exist" << std::endl;
	}
	return 0;
}