#include "FontManager.hpp"

const std::string& FontManager::get_path() {
	return font_path_;
}

void FontManager::set_path(const std::string& path) {
	font_path_ = path;
}
