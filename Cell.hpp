#pragma once

class Cell final {
public:
	Cell();

	void mine_cell();
	int try_open();
	int set_reset_flag();

	void clear();

	enum class cell_state { CLOSE, OPEN, FLAG };

	[[nodiscard]] cell_state check_state() const;
	[[nodiscard]] bool is_mined() const;

private:
	bool is_mined_;
	cell_state state_;
};