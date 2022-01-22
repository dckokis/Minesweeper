#pragma once

class Cell final {
public:
	Cell();

	void set_bomb();
	int open();
	int flag();

	void clear();

	enum class cellState { close, open, flag };

	[[nodiscard]] cellState condition() const;
	[[nodiscard]] bool is_mined() const;

	~Cell() = default;

private:
	bool bomb_;
	cellState condition_;
};