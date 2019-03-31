#include "utils.h"

#include <iostream>
#include <map>


sf::Texture& engine::texture_from_file(const std::filesystem::path& path) {
    static auto loaded = std::map<std::filesystem::path, sf::Texture>{};

    {
        auto cached = loaded.find(path);
        if (cached != loaded.end()) {
            std::cout << "Using cached " << path << " texture.\n";

            return cached->second;
        }
    }

    auto tex = sf::Texture{};

    if (not tex.loadFromFile(path)) {
        throw std::invalid_argument("Could not load texture: " + path.string());
    }

    loaded.emplace(std::make_pair(path, tex));

    std::cout << "Loaded " << path << " texture.\n";

    return loaded[path];
}

sf::Image engine::create_image(Size size, const sf::Color& color) {
    auto img = sf::Image{};
    img.create(size.width, size.height, color);
    return img;
}
