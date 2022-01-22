#include "GraphicElement.hpp"

namespace {
	std::vector<std::string> textures_names = {
		"close", "flag", "bomb", "red_bomb", "0", "1", "2", "3", "4", "5", "6", "7", "8", "small_flag", "clock",
		"smile", "win", "losing", "back"
	};
	std::unordered_map<std::string, sf::Texture> textures;
}


GraphicElement::GraphicElement() {
	for (const auto& name : textures_names) {
		sf::Texture texture;
		if (!texture.loadFromFile("images/" + name + ".png")) {
			throw std::invalid_argument(name);
		}
		textures[name] = texture;
	}
}

void GraphicElement::customize(sf::Sprite& sprite, const sf::Vector2i& elem_pos, const int& elem_size,
                               const std::string& elem_name) const
{
	const auto scale = static_cast<float> (elem_size) / textures.at(elem_name).getSize().x;
	sprite.setTexture(textures.at(elem_name));
	sprite.setScale(scale, scale);
	sprite.setPosition(static_cast<float>(elem_pos.x), static_cast<float>(elem_pos.y));
}