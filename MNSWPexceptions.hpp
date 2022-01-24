#pragma once
#include <exception>
#include <string>

class MNSWPexceptions final : std::exception {
private:
	std::string m_error;
public:
	explicit MNSWPexceptions(std::string error) : m_error(std::move(error)) {}

	[[nodiscard]] const char *what() const override {
		return m_error.c_str();
	}
};
