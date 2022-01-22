#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>

class GraphicElement final {
public:
	GraphicElement();
	void customize(sf::Sprite& sprite, const sf::Vector2i& elem_pos, const int& elem_size, const std::string& elem_name) const;
};