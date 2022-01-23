#include "Cell.hpp"

Cell::Cell() : is_mined_(false), state_(cell_state::CLOSE) {
}

void Cell::mine_cell() {
	is_mined_ = true;
}

int Cell::try_open() {
	if (state_ == cell_state::FLAG){
		return -1;
	}
	if (is_mined_ || state_ != cell_state::OPEN) {
		state_ = cell_state::OPEN;
		if (is_mined_) {
			return 0;
		}
		return 1;
	}
}

int Cell::set_reset_flag() {
	if (state_ == cell_state::FLAG) {
		state_ = cell_state::CLOSE;
		return 0;
	}
	if (state_ != cell_state::OPEN) {
		state_ = cell_state::FLAG;
		return 1;
	}
}

void Cell::clear() {
	is_mined_ = false;
	state_ = cell_state::CLOSE;
}

Cell::cell_state Cell::check_state() const {
	return state_;
}

bool Cell::is_mined() const {
	return is_mined_;
}