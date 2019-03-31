#ifndef GJAM_QUICK_ENGINE_UTILS_H
#define GJAM_QUICK_ENGINE_UTILS_H


#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>

#include "2d.h"

namespace engine {

sf::Texture& texture_from_file(const std::filesystem::path& path);
sf::Image create_image(Size size, const sf::Color& = {0, 0, 0, 0});

template <typename T>
auto from_file(const std::filesystem::path& path) {
    auto obj = T{};

    if (not obj.loadFromFile(path)) {
        throw std::invalid_argument("Could not load texture: " + path.string());
    }

    return obj;
}

}

#endif
