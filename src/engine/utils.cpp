#include "utils.h"


sf::Texture engine::texture_from_file(const std::filesystem::path& path) {
    auto tex = sf::Texture{};

    if (not tex.loadFromFile(path)) {
        throw std::invalid_argument("Could not load texture: " + path.string());
    }

    return tex;
}

sf::Image engine::create_image(Size size, const sf::Color& color) {
    auto img = sf::Image{};
    img.create(size.width, size.height, color);
    return img;
}
