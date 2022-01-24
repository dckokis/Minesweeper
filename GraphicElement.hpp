#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "MNSWPexceptions.hpp"

class GraphicElement final {
public:
	static void load_textures();
	static void customize(sf::Sprite& sprite, const sf::Vector2i& elem_pos, const int& elem_size, const std::string& elem_name);
};