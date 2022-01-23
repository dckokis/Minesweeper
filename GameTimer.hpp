#pragma once
#include <SFML/System/Clock.hpp>

struct GameTimer {
private:
	sf::Clock clock_;
	int time_of_round_{};
public:
	GameTimer() {
		clock_.restart();
	}
	sf::Time restart() 	{
		return clock_.restart();
	}

	int get_time() {
		time_of_round_ = static_cast<int>(clock_.getElapsedTime().asSeconds());
		time_of_round_ = time_of_round_ >= 1000 ? 999 : time_of_round_;
		return time_of_round_;
	}

};
