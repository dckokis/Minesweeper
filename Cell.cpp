#include "cell.hpp"

Cell::Cell() : bomb_(false), condition_(cellState::close) {
}

void Cell::set_bomb() {
	bomb_ = true;
}

int Cell::open() {
	if (condition_ == cellState::flag) {
		return -1;
	}
	if (bomb_ == true) {
		condition_ = cellState::open;
		return 0;
	}
	if (condition_ != cellState::open) {
		condition_ = cellState::open;
		return 1;
	}
	return -1;
}

int Cell::flag() {
	if (condition_ == cellState::flag) {
		condition_ = cellState::close;
		return 0;
	}
	if (condition_ != cellState::open) {
		condition_ = cellState::flag;
		return 1;
	}
	return -1;
}

void Cell::clear() {
	bomb_ = false;
	condition_ = cellState::close;
}

Cell::cellState Cell::condition() const {
	return condition_;
}

bool Cell::is_mined() const {
	return bomb_;
}